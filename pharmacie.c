#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure de données nécessaire pour  représentation d'un stock
typedef struct Medicament
{
    char libelle[30];
    float prix;
    int qte;
} Medicament;

typedef struct ListMedicament
{
    Medicament medoc;
    struct ListMedicament *suiv;
} ListMedoc, *ListMedicament;

// Ecrire la procédure vendre(MEd, NbBoites, St)
void vendre(char *med, int nbBoites, ListMedicament *st)
{
    int trouve = 0;
    if (*st == NULL)
        printf("Aucun medicament en stock\n");
    else if (strcmp(med, (*st)->medoc.libelle) == 0)
    {
        if ((*st)->medoc.qte > nbBoites)
        {
            (*st)->medoc.qte -= nbBoites;
            trouve = 1; // On a trouve le med et il reste encore en stock
        }
        else if ((*st)->medoc.qte == nbBoites)
        {
            trouve = 2; // On a trouve le med et le stock == 0
            ListMedicament f = *st;
            *st = (*st)->suiv;
            free(f);
        }
        else
        {
            trouve = 3; // stock insuffisant
        }
    }
    else
    {
        ListMedicament suiv = (*st)->suiv, pre = *st;
        while (suiv != NULL && trouve == 0)
        {
            if (strcmp(med, suiv->medoc.libelle) == 0)
            {
                if (suiv->medoc.qte > nbBoites)
                {
                    (*st)->medoc.qte -= nbBoites;
                    trouve = 1; // On a trouve le med et il reste encore en stock
                }
                else if ((*st)->medoc.qte == nbBoites)
                {
                    trouve = 2; // On a trouve le med et le stock == 0
                    suiv = suiv->suiv;
                    pre->suiv->suiv = NULL;
                    free(pre->suiv);
                    pre->suiv = suiv;
                }
                else
                {
                    trouve = 3; // stock insuffisant
                }
                break;
            }
            suiv = suiv->suiv;
            pre = pre->suiv;
        }
    }
    switch (trouve)
    {
    case 0:
        printf("\nCe medicament n'est pas en stock\n");
        break;
    case 1:
        printf("\nVente Ok\n");
        break;
    case 2:
        printf("\nVente Ok mais le medicament n'est plus en stock\n");
        break;

    default:
        printf("\nstock insuffisant\n");
        break;
    }
}

// Ecrire la procédure acheter(Med, nbBoites, prix)
void acheter(char *med, int nbBoites, float prix, ListMedicament *st)
{
    ListMedicament l = *st;
    while (l != NULL)
    {
        if (strcmp(med, l->medoc.libelle) == 0)
        {
            l->medoc.prix = prix;
            l->medoc.qte += nbBoites;
            break;
        }
        else
        {
            l = l->suiv;
        }
    }
    if (l == NULL)
    {
        ListMedicament medoc = (ListMedicament)malloc(sizeof(ListMedoc));
        medoc->medoc.prix = prix;
        medoc->medoc.qte = nbBoites;
        strcpy(medoc->medoc.libelle, med);
        medoc->suiv = *st;
        *st = medoc;
        medoc = NULL;
    }
}

// Ecrire une prix stock
float prixStock(ListMedicament st)
{
    float prixTotal = 0;
    ListMedicament l = st;
    while (l != NULL)
    {
        prixTotal += (l->medoc.prix * l->medoc.qte);
        l = l->suiv;
    }
    return prixTotal;
}

int main(void)
{
    ListMedicament m = NULL;
    acheter("Doliprame", 10, 5400, &m); // test 1
    acheter("Doliprame", 1, 5000, &m);  // test 2
    vendre("Para", 12, &m);             // test 3
    acheter("Para", 50, 100, &m);       // test 4
    printf("Prix total = %.2fFCFA\n", prixStock(m));
    return 0;
}