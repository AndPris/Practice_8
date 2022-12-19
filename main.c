#include <stdio.h>
#include <math.h>

typedef struct Complex {
    float re;
    float im;
} complex;

void noCharShort(short *var) { //checking for characters
    char cond, ch;
    do {
        cond = 0;
        scanf("%hd%c", var, &ch);

        if(ch != '\n') {
            printf("Invalid data\n");
            cond = 1;
            fflush(stdin);
        }
    } while(cond);

}

short choice(short *var) {
    char cond;
    do {
        cond = 0;
        noCharShort(var);

        if(*var != 1 && *var != 2 && *var != 3 && *var != 4) {
            printf("You have to enter 1, 2, 3 or 4\n");
            fflush(stdin);
            cond = 1;
        }
    } while(cond);
}

void floatInput(float *var) { //checking for characters and >=0
    char cond, ch;
    do {
        cond = 0;
        scanf("%f%c", var, &ch);

        if(ch != '\n' || *var < 0) {
            printf("Invalid data\n");
            cond = 1;
            fflush(stdin);
        }
    } while(cond);

}

void borders(float *low, float *top) {
    char cond;
    do {
        cond = 0;
        printf("fmin:\n");
        floatInput(low);
        printf("fmax:\n");
        floatInput(top);

        if(*low >= *top) {
            printf("fmin is higher then fmax or equals to it\n");
            fflush(stdin);
            cond = 1;
        }
    } while(cond);
}

complex div(complex z1, complex z2) { // division of two complex numbers
    complex z;
    z.re = (z1.re * z2.re + z1.im * z2.im) / (z2.re * z2.re + z2.im * z2.im);
    z.im = (z1.im * z2.re - z1.re * z2.im) / (z2.re * z2.re + z2.im * z2.im);
    return z;
}

complex firstScheme (float R1, float R2, float L, float C, float omega) {
    complex z1, z2;

    z1.re = L/C;
    z1.im = (-1) * R1 / (omega * C);

    z2.re = R1;
    z2.im = omega * L - 1 / (omega * C);

    return div(z1, z2);
}

complex secondScheme (float R1, float R2, float L, float C, float omega) {
    complex z1, z2;

    z1.re = L/C;
    z1.im = R1 / (omega * C);

    z2.re = R1;
    z2.im = omega * L - 1 / (omega * C);

    return div(z1, z2);
}

complex thirdScheme (float R1, float R2, float L, float C, float omega) {
    complex z1, z2;

    z1.re = R1 * R2;
    z1.im = R1 * (omega * L - 1 / (omega * C));

    z2.re = R1 + R2;
    z2.im = omega * L - 1 / (omega * C);

    return div(z1, z2);
}

complex fourthScheme (float R1, float R2, float L, float C, float omega) {
    complex z1, z2;

    z1.re = R1 * R2 + L/C;
    z1.im = omega * L * R1 - R2 / (omega * C);

    z2.re = R1 + R2;
    z2.im = omega * L - 1 / (omega * C);

    return div(z1, z2);
}

void displayComplex(complex z) {
    if(z.im >= 0) {
        printf("%f + i*%f", z.re, z.im);
    } else {
        printf("%f - i*%f", z.re, fabs(z.im));
    }
}

void compute(complex (*scheme) (float, float, float, float, float), float R1, float R2, float L, float C, float fmin, float fmax, float df) {
    float omega, f0;
    complex z;
    printf("\nFrequency\t\tResonance frequency\t\tResistance\n");
    while (fmin <= fmax) {
        omega = 2 * M_PI * fmin;
        f0 = 1 / (2 * M_PI * sqrt(L*C));
        z = (*scheme)(R1, R2, L, C, omega);
        printf("%7.2f%28.2f\t\t\t", fmin, f0);
        displayComplex(z);
        printf("\n");
        fmin += df;
    }
}

int main() {
    float R1, R2, L, C, fmin, fmax, df;
    short scheme;
    printf("Enter the number of scheme (1, 2, 3 or 4):");
    choice(&scheme);
    printf("R1:");
    floatInput(&R1);
    printf("R2:");
    floatInput(&R2);
    printf("L:");
    floatInput(&L);
    printf("C:");
    floatInput(&C);
    borders(&fmin, &fmax);
    printf("df:");
    floatInput(&df);

    switch (scheme) {
        case 1:
            compute(firstScheme, R1, R2, L, C, fmin, fmax, df);
            break;
        case 2:
            compute(secondScheme, R1, R2, L, C, fmin, fmax, df);
            break;
        case 3:
            compute(thirdScheme, R1, R2, L, C, fmin, fmax, df);
            break;
        case 4:
            compute(fourthScheme, R1, R2, L, C, fmin, fmax, df);
            break;
    }

    return 0;
}