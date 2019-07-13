#include "lss_34_03.h"

const double DBL_EPSILON = 2.2204460492503131e-016;

// This function swaps values pointed by ap and bp
void swap(int *ap, int *bp)
{
    int temp = *ap;
    *ap = *bp;
    *bp = temp;
}

int lss_34_03(int n, double* A, double* B, double* X, double* tmp) {
    char singular_matrix = 0;
    int i, j, k, max_row, max_col, *indi = (int *) tmp, *indj = (int *) tmp + n, free_variables = 0;
    double cur_sum, first_coef = 0.0, max_elem = 0.0;

    for(i = 0; i < n; i++)
    {
        indi[i] = i;
        indj[i] = i;
    }

    // forward elimination
    printf("Forward elimination:\n");

    for (k = 0; k < n; k++)
    {
        printf("step #%d\n", k+1);

        for (i = k; i < n; i++)
        {
            for (j = k; j < n; j++)
            {
                if (fabs(A[indi[i]*n + indj[j]]) - max_elem > DBL_EPSILON)
                {
                    max_elem = fabs(A[indi[i]*n + indj[j]]),
                    max_row = i,
                    max_col = j;
                }
            }
        }

        if (max_elem < DBL_EPSILON)
        {
            singular_matrix = 1;
            free_variables = n - k;
            break;
        }


        // swap rows & columns
        swap(&indj[k], &indj[max_col]);
        swap(&indi[k], &indi[max_row]);

        max_elem = A[indi[k]*n + indj[k]];

        for (i = k; i < n; i++)
            A[indi[k]*n + indj[i]] /= max_elem;
        B[indi[k]] /= max_elem;

        max_elem = 0.0;


        for(i = k+1; i<n; i++) {
            first_coef = A[indi[i]*n + indj[k]];
            for (j = k; j < n; j++)
            {
                A[indi[i]*n + indj[j]] -=  first_coef * A[indi[k]*n + indj[j]];
            }
            B[indi[i]] -= first_coef * B[indi[k]];
        }

        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                printf("%1.9lf ", A[indi[i]*n + indj[j]]);
            }
            printf("%1.9lf ", B[indi[i]]);
            printf("\n");
        }
        printf("\n");
    }

    // back substitution
    printf("Back substitution:\n");

    if(singular_matrix)
    {
        for(i = n-1; i >= n - free_variables; i--)
        {
            X[indj[i]] = 0.0;
            if(fabs(B[indi[i]]) >= DBL_EPSILON)
            {
                return 1;
            }
        }
    }

    for (i = n - 1 - free_variables; i >= 0; i--)
    {
        cur_sum = 0.0;
        for (j = i + 1; j < n; j++)
        {
            cur_sum += A[indi[i]*n + indj[j]] * X[indj[j]];
        }
        X[indj[i]] = (B[indi[i]] - cur_sum) / A[indi[i]*n + indj[i]]; // always non zero
    }

    for (i = 0; i < n; i++)
    {
        printf("%1.9lf\n", X[i]);
    }
    return 0;
}

size_t lss_memsize_34_03(int n) {
    return sizeof(int) * 2 * n;
}