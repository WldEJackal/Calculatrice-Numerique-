#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define HISTORIQUE_MAX 100

// Définir la structure de la pile pour l'historique des calculs
typedef struct {
    char historique[HISTORIQUE_MAX][200];  // Tableau pour stocker les chaînes d'historique
    int sommet;  // Index du sommet de la pile
} PileHistoriqueCalcul;

// Initialiser la pile
void init_pile(PileHistoriqueCalcul* pile) {
    pile->sommet = -1;
}

// Ajouter un nouvel historique de calcul dans la pile
void empiler_historique_calcul(PileHistoriqueCalcul* pile, const char* historique) {
    if (pile->sommet < HISTORIQUE_MAX - 1) {
        pile->sommet++;
        strncpy(pile->historique[pile->sommet], historique, sizeof(pile->historique[pile->sommet]) - 1);
        pile->historique[pile->sommet][sizeof(pile->historique[pile->sommet]) - 1] = '\0';  // Null-terminer la chaîne
    } else {
        printf("La pile est pleine, impossible d'ajouter plus d'historique de calcul.\n");
    }
}

// Écrire l'historique des calculs dans un fichier
void ecrire_historique_calcul_dans_fichier(const PileHistoriqueCalcul* pile, const char* nom_fichier) {
    FILE* fichier = fopen(nom_fichier, "a");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", nom_fichier);
        return;
    }

    for (int i = 0; i <= pile->sommet; i++) {
        fprintf(fichier, "%s\n", pile->historique[i]);
    }

    fclose(fichier);
}

typedef struct NoeudHistoriqueConversion {
    char historique[200];  // Chaîne représentant l'historique d'une conversion
    struct NoeudHistoriqueConversion* suivant;  // Pointeur vers le noeud suivant
} NoeudHistoriqueConversion;

// Fonction pour créer un nouveau noeud pour l'historique de conversion
NoeudHistoriqueConversion* creer_noeud_historique_conversion(const char* historique) {
    NoeudHistoriqueConversion* nouveau_noeud = (NoeudHistoriqueConversion*)malloc(sizeof(NoeudHistoriqueConversion));
    if (nouveau_noeud == NULL) {
        printf("Erreur d'allocation de mémoire pour le noeud d'historique de conversion.\n");
        return NULL;
    }
    strncpy(nouveau_noeud->historique, historique, sizeof(nouveau_noeud->historique) - 1);
    nouveau_noeud->historique[sizeof(nouveau_noeud->historique) - 1] = '\0';  // Null-terminer la chaîne
    nouveau_noeud->suivant = NULL;
    return nouveau_noeud;
}

// Fonction pour ajouter un nouvel historique de conversion à la liste chaînée
void empiler_historique_conversion(NoeudHistoriqueConversion** tete, const char* historique) {
    NoeudHistoriqueConversion* nouveau_noeud = creer_noeud_historique_conversion(historique);
    if (nouveau_noeud != NULL) {
        nouveau_noeud->suivant = *tete;  // Le nouveau noeud pointe vers l'ancien "tête"
        *tete = nouveau_noeud;  // Mise à jour de la tête pour pointer vers le nouveau noeud
    }
}

// Écrire l'historique des conversions dans un fichier
void ecrire_historique_conversion_dans_fichier(NoeudHistoriqueConversion* tete, const char* nom_fichier) {
    FILE* fichier = fopen(nom_fichier, "a");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", nom_fichier);
        return;
    }

    NoeudHistoriqueConversion* courant = tete;
    while (courant != NULL) {
        fprintf(fichier, "%s\n", courant->historique);
        courant = courant->suivant;
    }

    fclose(fichier);
}

// Libérer la mémoire de la liste chaînée
void liberer_historique_conversion(NoeudHistoriqueConversion* tete) {
    NoeudHistoriqueConversion* courant = tete;
    while (courant != NULL) {
        NoeudHistoriqueConversion* temp = courant;
        courant = courant->suivant;
        free(temp);
    }
}

// Prototypes des fonctions
char* calculatrice(char* num1, char operation, char* num2, int base);
int char_vers_chiffre(char c);
int est_numero_valide(const char* num_str, int base);
void convertir_partie_entiere_en_decimale(const char* num_str, int base, double* partie_entiere);
void convertir_partie_fractionnaire_en_decimale(const char* num_str, int base, double* partie_fractionnaire);
void convertir_de_base_en_decimale(const char* num_str, int base, double* resultat);
void convertir_partie_entiere_de_decimale(double decimal, int base, char* resultat);
void convertir_partie_fractionnaire_de_decimale(double decimal, int base, char* resultat, int precision);
void convertir_en_base(double decimal, int base, char* resultat, int precision);
void ecrire_dans_historique(const char* nom_fichier, const char* historique);

int main() {
    printf("BIENVENUE DANS VOTRE CALCULATRICE NUMERIQUE AVANCEE !\n");
    char choix[4] = "oui";
    char mode[12];
    while (strcmp(choix, "oui") == 0) {
        printf("VEUILLEZ CHOISIR ENTRE LE MODE CONVERSION OU LE MODE CALCUL.\n");
        scanf("%s", mode);

        if (strcmp(mode, "calcul") == 0) {
            char num1[100], num2[100], operation;
            int base;

            // Demander à l'utilisateur de saisir les valeurs
            printf("Entrez un nombre réel : ");
            scanf("%s", num1);

            printf("Entrez la base du nombre (par exemple, 10 pour décimal, 2 pour binaire, 8 pour octal, 16 pour hexadécimal): ");
            scanf("%d", &base);

            // Vérification de la validité du nombre dans la base donnée
            if (!est_numero_valide(num1, base)) {
                printf("Le nombre n'est pas valide pour la base %d.\n", base);
                return 1;
            }

            printf("Entrez l'opération (+, -, *, /): ");
            scanf(" %c", &operation);

            printf("Entrez un autre nombre réel: ");
            scanf("%s", num2);

            // Vérification de la validité du second nombre dans la base donnée
            if (!est_numero_valide(num2, base)) {
                printf("Le nombre n'est pas valide pour la base %d.\n", base);
                return 1;
            }

            // Effectuer le calcul
            char* resultat = calculatrice(num1, operation, num2, base);
            printf("Résultat: %s\n", resultat);

            // Écrire dans l'historique des calculs
            char historique[200];
            snprintf(historique, sizeof(historique), "%s %c %s = %s\n", num1, operation, num2, resultat);
            ecrire_dans_historique("calcul1.txt", historique);

        } else if (strcmp(mode, "conversion") == 0) {
            char num_str[100];
            int base_de, base_vers;

            // Demander à l'utilisateur d'entrer un nombre et ses bases
            printf("Entrez un nombre: ");
            scanf("%s", num_str);

            printf("Entrez la base du nombre (par exemple, 2 pour binaire, 8 pour octal, 10 pour décimal, 16 pour hexadécimal): ");
            scanf("%d", &base_de);

            if (!est_numero_valide(num_str, base_de)) {
                printf("Le nombre n'est pas valide pour la base %d.\n", base_de);
                return 1;
            }

            printf("Entrez la base cible (par exemple, 2 pour binaire, 8 pour octal, 10 pour décimal, 16 pour hexadécimal): ");
            scanf("%d", &base_vers);

            // Convertir le nombre de la base source en base décimale
            double valeur_decimal;
            convertir_de_base_en_decimale(num_str, base_de, &valeur_decimal);

            // Convertir du décimal vers la base cible
            char resultat[100];
            convertir_en_base(valeur_decimal, base_vers, resultat, 2);  // 2 chiffres après la virgule

            printf("Le nombre %s en base %d est %s en base %d.\n", num_str, base_de, resultat, base_vers);

            // Écrire dans l'historique des conversions
            char historique[200];
            snprintf(historique, sizeof(historique), "%s base %d -> %s base %d\n", num_str, base_de, resultat, base_vers);
            ecrire_dans_historique("conversion.txt", historique);

        } else {
            printf("Mode non reconnu !\n");
        }

        printf("VOULEZ-VOUS POURSUIVRE AVEC UNE AUTRE OPÉRATION (oui/non) ? ");
        scanf("%s", choix);
        while (strcmp(choix, "oui") != 0 && strcmp(choix, "non") != 0) {
            printf("Choix non reconnu! Veuillez confirmer votre choix: ");
            scanf("%s", choix);
        }
    }

    if (strcmp(choix, "non") == 0) {
        printf("MERCI D'AVOIR UTILISÉ LA CALCULATRICE NUMÉRIQUE. À LA PROCHAINE !!\n");
    }

    return 0;
}

char* calculatrice(char* num1, char operation, char* num2, int base) {
    static char res[100];  // Utilisation d'un tableau statique pour retourner le résultat
    double resultat;
    convertir_de_base_en_decimale(num1, base, &resultat);  // Convertir num1 en décimal

    double num2_value;
    convertir_de_base_en_decimale(num2, base, &num2_value); // Convertir num2 en décimal

    switch (operation) {
        case '+':
            resultat += num2_value;
            break;
        case '-':
            resultat -= num2_value;
            break;
        case '*':
            resultat *= num2_value;
            break;
        case '/':
            if (num2_value != 0) {
                resultat /= num2_value;
            } else {
                strcpy(res, "Erreur : Division par zéro.\n");
                return res;  // Retourner l'erreur si division par zéro
            }
            break;
        default:
            strcpy(res, "Opération invalide.\n");
            return res;  // Retourner l'erreur si l'opération est invalide
    }

    // Convertir le résultat en chaîne dans la base spécifiée
    convertir_en_base(resultat, base, res, 2); // Précision de 2 chiffres après la virgule
    return res;
}

int char_vers_chiffre(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    return -1; // Erreur si caractère invalide
}

int est_numero_valide(const char* num_str, int base) {
    int point_trouve = 0;
    for (int i = 0; i < strlen(num_str); i++) {
        char c = num_str[i];
        if (c == '.') {
            if (point_trouve) {
                return 0; // Si un autre point est trouvé, le nombre est invalide
            }
            point_trouve = 1;
        } else {
            int valeur_chiffre = char_vers_chiffre(c);
            if (valeur_chiffre == -1 || valeur_chiffre >= base) {
                return 0;
            }
        }
    }
    return 1;
}

void convertir_partie_entiere_en_decimale(const char* num_str, int base, double* partie_entiere) {
    *partie_entiere = 0;
    int longueur = strlen(num_str);
    for (int i = 0; i < longueur; i++) {
        int valeur_chiffre = char_vers_chiffre(num_str[i]);
        *partie_entiere += valeur_chiffre * pow(base, longueur - i - 1);
    }
}

void convertir_partie_fractionnaire_en_decimale(const char* num_str, int base, double* partie_fractionnaire) {
    *partie_fractionnaire = 0;
    int longueur = strlen(num_str);
    for (int i = 0; i < longueur; i++) {
        int valeur_chiffre = char_vers_chiffre(num_str[i]);
        *partie_fractionnaire += valeur_chiffre * pow(base, -(i + 1));
    }
}

void convertir_de_base_en_decimale(const char* num_str, int base, double* resultat) {
    char* point = strchr(num_str, '.');
    if (point != NULL) {
        char partie_entiere[64], partie_fractionnaire[64];
        int longueur_partie_entiere = point - num_str;
        strncpy(partie_entiere, num_str, longueur_partie_entiere);
        partie_entiere[longueur_partie_entiere] = '\0';
        strcpy(partie_fractionnaire, point + 1);

        double valeur_entiere = 0;
        double valeur_fractionnaire = 0;

        convertir_partie_entiere_en_decimale(partie_entiere, base, &valeur_entiere);
        convertir_partie_fractionnaire_en_decimale(partie_fractionnaire, base, &valeur_fractionnaire);

        *resultat = valeur_entiere + valeur_fractionnaire;
    } else {
        convertir_partie_entiere_en_decimale(num_str, base, resultat);
    }
}

void convertir_partie_entiere_de_decimale(double decimal, int base, char* resultat) {
    int partie_entiere = (int)decimal;
    int index = 0;
    if (partie_entiere == 0) {
        resultat[index++] = '0';
    } else {
        while (partie_entiere > 0) {
            int reste = partie_entiere % base;
            if (reste < 10)
                resultat[index++] = '0' + reste;
            else
                resultat[index++] = 'A' + (reste - 10);
            partie_entiere /= base;
        }
    }
    resultat[index] = '\0';
    for (int i = 0; i < index / 2; i++) {
        char temp = resultat[i];
        resultat[i] = resultat[index - i - 1];
        resultat[index - i - 1] = temp;
    }
}

void convertir_partie_fractionnaire_de_decimale(double decimal, int base, char* resultat, int precision) {
    int index = 0;
    while (precision > 0) {
        decimal *= base;
        int chiffre = (int)decimal;
        if (chiffre < 10)
            resultat[index++] = '0' + chiffre;
        else
            resultat[index++] = 'A' + (chiffre - 10);
        decimal -= chiffre;
        precision--;
    }
    resultat[index] = '\0';
}

void convertir_en_base(double decimal, int base, char* resultat, int precision) {
    char partie_entiere[64], partie_fractionnaire[64];
    convertir_partie_entiere_de_decimale(decimal, base, partie_entiere);
    convertir_partie_fractionnaire_de_decimale(decimal - (int)decimal, base, partie_fractionnaire, precision);
    strcpy(resultat, partie_entiere);
    if (precision > 0) {
        strcat(resultat, ".");
        strcat(resultat, partie_fractionnaire);
    }
}

void ecrire_dans_historique(const char* nom_fichier, const char* historique) {
    FILE* fichier = fopen(nom_fichier, "a");
    if (fichier == NULL) {
        printf("Erreur d'ouverture du fichier %s\n", nom_fichier);
        return;
    }
    fprintf(fichier, "%s", historique);
    fclose(fichier);
}
