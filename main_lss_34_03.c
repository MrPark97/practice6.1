#include "lss_34_03.h"

char error_msgs[][MAX_STRING_LENGTH] = {
        "",
        "",
        "using -h, -? options error", // begins from 2 because of task format
        "undefined option error",
        "wrong interface parameters error",
        "cannot open output file error",
        "cannot open input file error",
        "reading data error"
};


int main(int argc, char* argv[])
{
    clock_t end_time, start_time = clock();
    int i, j, n, result;
    double *A, *B, *x, *tmp, execution_time;
    FILE *input_file, *output_file;
    char *input_filename = "lss_34_03_in.txt", *output_filename = "lss_34_03_out.txt";

    char FLAG_D = 0, FLAG_E = 0, FLAG_P = 0, FLAG_T = 0; // flags (cmd options)
    char FLAG_F = 0; // custom filenames;

    for (i = 1; i < argc; ++i)
    {
        if (argv[i][0] == '-' && argv[i][2] == '\0')
            switch (argv[i][1])
            {
                case 'd':
                    FLAG_D = 1;
                    break;
                case 'e':
                    FLAG_E = 1;
                    break;
                case 'p':
                    FLAG_P = 1;
                    break;
                case 't':
                    FLAG_T = 1;
                    break;
                case 'h':;
                case '?':
                    printf("Usage: lss [input_file_name] [output_file_name] [options]\n"
                           "Where options include:\n"
                           "-d    print debug messages [default OFF]\n"
                           "-e    print errors [default OFF]\n"
                           "-p    print matrix [default OFF]\n"
                           "-t    print execution t [default OFF]\n"
                           "-h, -?     print this and exit\n"
                           "Default input_file_name value is lss_34_03_in.txt, default output_file_name value is lss_34_03_out.txt.\n");
                    return 2;
                default:
                    return 3;
            }
        else
        {
            if (i == 1)
                input_filename = argv[i], FLAG_F = 1;
            else if (i == 2 && FLAG_F)
                output_filename = argv[i];
            else
            {
                return 4;
            }
        }
    }

    if ((output_file = fopen(output_filename, "w")) == NULL)
    {
        if(FLAG_E)
            fprintf(stderr, "%s\n", error_msgs[5]);
        return 5;
    }

    if ((input_file = fopen(input_filename, "r")) == NULL)
    {
        if(FLAG_E)
            fprintf(stderr, "%s\n", error_msgs[6]);
        fclose(output_file);
        return 6;
    }

    if (fscanf(input_file, "%d", &n) != 1)
    {
        if(FLAG_E)
            fprintf(stderr, "%s\n", error_msgs[7]);
        fclose(output_file);
        fclose(input_file);
        return 7;
    }

    A = (double *) malloc(n * n * sizeof(double)),
    B = (double *) malloc(n * sizeof(double)),
    x = (double *) calloc(n, sizeof(double)),
    tmp = (double *) malloc(lss_memsize_34_03(n));

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            if (fscanf(input_file, "%lf", &A[i * n + j]) != 1)
            {
                if(FLAG_E)
                    fprintf(stderr, "%s\n", error_msgs[7]);
                fclose(output_file);
                fclose(input_file);
                return 7;
            }

    for (i = 0; i < n; i++)
        if (fscanf(input_file, "%lf", &B[i]) != 1)
        {
            if(FLAG_E)
                fprintf(stderr, "%s\n", error_msgs[7]);
            fclose(output_file);
            fclose(input_file);
            return 7;
        }

    if (FLAG_P)
    {
        printf("%d\n", n);

        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
                printf("%1.9lf ", A[i * n + j]);
            printf("\n");
        }

        for (int i = 0; i < n; i++)
            printf("%1.9lf\n", B[i]);

        printf("\n");
    }


    result = lss_34_03(n, A, B, x, tmp);

    end_time = clock();
    execution_time = (double)((end_time - start_time)) / CLOCKS_PER_SEC;

    if (FLAG_T)
        printf("Execution time: %1.9lf \n", execution_time);

    if(result == 0)
    {
        fprintf(output_file, "%d\n", n);

        for (i = 0; i < n; ++i)
            fprintf(output_file, "%1.9lf\n", x[i]);
    }
    else
        fprintf(output_file, "0");

    fclose(input_file);
    fclose(output_file);
    free(A);
    free(B);
    free(x);
    free(tmp);
    return result;
}