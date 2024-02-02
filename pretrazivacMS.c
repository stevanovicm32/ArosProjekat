#include <stdio.h>
#include <string.h>

#define CRVENA "\033[0;31m"
#define MAX_DUZINA 1024
#define RESET "\033[0m"

int main(int argc, char *argv[])
{
    char red[MAX_DUZINA];
    if (argc != 3)
    {
        fprintf(stderr, "Kako koristiti funkciju?: %s <trazena rec> <ime fajla>\n", argv[0]);
        return 1;
    }
    char *trazenaRec = argv[1];
    char *imeFajla = argv[2];

    FILE *datoteka = fopen(imeFajla, "r");
    if (datoteka == NULL)
    {
        printf("Nije moguće otvoriti datoteku.\n");
        return 1;
    }

    printf("Rezultati pretrage:\n");

    int brojReda = 1; // broj trenutne linije
    while (fgets(red, MAX_DUZINA, datoteka))
    {
        if (strstr(red, trazenaRec) != NULL)
        {
            printf("Linija %d: ", brojReda);

            char *found = strstr(red, trazenaRec);
            while (found != NULL)
            {
                // ispis svih reci pre pronadjene
                fwrite(red, 1, found - red, stdout);

                // ispsis reci u crvenoj boji
                printf(CRVENA);
                fwrite(found, 1, strlen(trazenaRec), stdout);
                printf(RESET);

                strcpy(red, found + strlen(trazenaRec));

                // ako postoji novo pojavljivanje trazene reci, ovde ce se sacuvati i while petlja ce se nastaviti
                found = strstr(red, trazenaRec);
            }

            printf("%s", red);
        }

        brojReda++;
    }

    fclose(datoteka);

    return 0;
}
