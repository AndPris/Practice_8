#include <stdio.h>
#include <math.h>

typedef struct Complex {
    double re;
    double im;
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

void doubleInput(double *var) { //checking for characters and >=0
    char cond, ch;
    do {
        cond = 0;
        scanf("%lf%c", var, &ch);

        if(ch != '\n' || *var <= 0 || *var > 1e10) {
            printf("Invalid data\n");
            cond = 1;
            fflush(stdin);
        }
    } while(cond);

}

void borders(double *low, double *top) {
    char cond;
    do {
        cond = 0;
        printf("fmin:");
        doubleInput(low);
        printf("fmax:");
        doubleInput(top);

        if(*low >= *top) {
            printf("fmin is higher then fmax or equals to it\n");
            fflush(stdin);
            cond = 1;
        }
    } while(cond);
}

void step(double *var, double range) {
    char cond;
    do {
        cond = 0;
        doubleInput(var);

        if(*var > range) {
            printf("df can't be higher then %g\n", range);
            fflush(stdin);
            cond = 1;
            continue;
        }

        if(*var == 0) {
            printf("df can't be 0\n", range);
            fflush(stdin);
            cond = 1;
            continue;
        }
    } while(cond);
}

complex div(complex z1, complex z2) { // division of two complex numbers
    complex z;
    z.re = (z1.re * z2.re + z1.im * z2.im) / (z2.re * z2.re + z2.im * z2.im);
    z.im = (z1.im * z2.re - z1.re * z2.im) / (z2.re * z2.re + z2.im * z2.im);
    return z;
}

complex firstScheme (double R, double L, double C, double omega) {
    complex z1, z2;

    z1.re = L/C;
    z1.im = (-1) * R / (omega * C);

    z2.re = R;
    z2.im = omega * L - 1 / (omega * C);

    return div(z1, z2);
}

complex secondScheme (double R, double L, double C, double omega) {
    complex z1, z2;

    z1.re = L/C;
    z1.im = R / (omega * C);

    z2.re = R;
    z2.im = omega * L - 1 / (omega * C);

    return div(z1, z2);
}

complex thirdScheme (double R1, double R2, double L, double C, double omega) {
    complex z1, z2;

    z1.re = R1 * R2;
    z1.im = R1 * (omega * L - 1 / (omega * C));

    z2.re = R1 + R2;
    z2.im = omega * L - 1 / (omega * C);

    return div(z1, z2);
}

complex fourthScheme (double R1, double R2, double L, double C, double omega) {
    complex z1, z2;

    z1.re = R1 * R2 + L/C;
    z1.im = omega * L * R1 - R2 / (omega * C);

    z2.re = R1 + R2;
    z2.im = omega * L - 1 / (omega * C);

    return div(z1, z2);
}

void displayComplex(complex z) {
    if(z.im >= 0) {
        printf("%g + i*%g", z.re, z.im);
    } else {
        printf("%g - i*%g", z.re, fabs(z.im));
    }
}

void compute1_2(complex (*scheme) (double, double, double, double), double R, double L, double C, double fmin, double fmax, double df) {
    double omega, f0;
    complex z;
    f0 = 1 / (2 * M_PI * sqrt(L*C));
    printf("Resonance frequency: %e", f0);
    printf("\nFrequency\tResistance\n");
    while (fmin < fmax) {
        omega = 2 * M_PI * fmin;
        z = (*scheme)(R, L, C, omega);
        printf("%7g\t\t", fmin);
        displayComplex(z);
        printf("\n");
        fmin += df;
    }
    omega = 2 * M_PI * fmax;
    z = (*scheme)(R, L, C, omega);
    printf("%7g\t\t", fmax);
    displayComplex(z);
    printf("\n");
}

void compute3_4(complex (*scheme) (double, double, double, double, double), double R1, double R2, double L, double C, double fmin, double fmax, double df) {
    double omega, f0;
    complex z;
    f0 = 1 / (2 * M_PI * sqrt(L*C));
    printf("Resonance frequency: %e", f0);
    printf("\nFrequency\tResistance\n");
    while (fmin <= fmax) {
        omega = 2 * M_PI * fmin;
        z = (*scheme)(R1, R2, L, C, omega);
        printf("%7g\t\t", fmin);
        displayComplex(z);
        printf("\n");
        fmin += df;
    }
    omega = 2 * M_PI * fmax;
    z = (*scheme)(R1, R2, L, C, omega);
    printf("%7g\t\t", fmax);
    displayComplex(z);
    printf("\n");
}

int main() {
    double R, R1, R2, L, C, fmin, fmax, df;
    short scheme;
    printf("Enter the number of scheme (1, 2, 3 or 4):");
    choice(&scheme);
    if (scheme == 1 || scheme == 2) {
        printf("R:");
        doubleInput(&R);
    } else {
        printf("R1:");
        doubleInput(&R1);
        printf("R2:");
        doubleInput(&R2);
    }
    printf("L:");
    doubleInput(&L);
    printf("C:");
    doubleInput(&C);
    borders(&fmin, &fmax);
    printf("df:");
    step(&df, fmax-fmin);

    switch (scheme) {
        case 1:
            compute1_2(firstScheme, R, L, C, fmin, fmax, df);
            break;
        case 2:
            compute1_2(secondScheme, R, L, C, fmin, fmax, df);
            break;
        case 3:
            compute3_4(thirdScheme, R1, R2, L, C, fmin, fmax, df);
            break;
        case 4:
            compute3_4(fourthScheme, R1, R2, L, C, fmin, fmax, df);
            break;
    }

    return 0;
}
