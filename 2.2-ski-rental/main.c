/**************************************************
 *
 * Fitxer: Pràctica 2.2
 * Propòsit: Programa principal del sistema LS Ski.
 * Autor: Judith Caralt
 * Data de creació: 15/05/2025
 * Data última modificació: 19/05/2025
 *
 **************************************************/

#include "linkedlist.h"                                                         // Llibreria pròpia per gestionar llistes enllaçades
#include <stdio.h>                                                              // Llibreria per entrada/sortida
#include <stdlib.h>                                                             // Llibreria per funcions bàsiques (malloc, free, atoi, etc.)
#include <string.h>                                                             // Llibreria per treballar amb cadenes de caràcters

#define MAX_LINE 256                                                            // Longitud màxima d'una línia llegida del fitxer
#define MAX_ITEMS 100                                                           // Nombre màxim d'ítems a l'inventari

/***********************************************
 * @Finalitat: Analitza una cadena amb format DD/MM/YYYY
 * @Paràmetres: in: dateStr = cadena de data.
 * @Retorn: struct Date amb els valors desglossats.
 ***********************************************/
Date parseDate(char dateStr[]) {
  Date d;
  int i, part, index;
  char buffer[5];

  // Inicialitzem la data com invàlida
  d.day = -1;
  d.month = -1;
  d.year = -1;

  i = 0;
  part = 0;
  index = 0;

  // Processem la cadena de text, partint-la per '/' o fi de línia
  while (dateStr[i] != ' ' && part < 3) {
    if (dateStr[i] == '/' || dateStr[i + 1] == ' ') {
      if (dateStr[i + 1] == ' ') {
        buffer[index] = dateStr[i];                                             // Inclou últim caràcter
        index = index + 1;
      }
      buffer[index] = ' ';                                                      // Finalitza la cadena temporal
      if (part == 0) {
        d.day = atoi(buffer);                                                   // Primer bloc → dia
      } else {
        if (part == 1) {
          d.month = atoi(buffer);                                               // Segon bloc → mes
        } else {
          d.year = atoi(buffer);                                                // Tercer bloc → any
        }
      }
      part = part + 1;
      index = 0;
    } else {
      buffer[index] = dateStr[i];                                               // Copiem caràcter
      index = index + 1;
    }
    i = i + 1;
  }

  // Comprovem validesa de la data
  if (d.day < 1 || d.month < 1 || d.year < 0) {
    d.day = -1;
    d.month = -1;
    d.year = -1;
  }

  return d;
}

/***********************************************
 * @Finalitat: Valida si la cadena de text és un format de data vàlid.
 * @Paràmetres: in: data = cadena amb la data.
 * @Retorn: 1 si el format és correcte, 0 si no.
 ***********************************************/
int validarFormatData(char *data) {
  int i, correcte;
  correcte = 1;

  // Comprovem longitud exacta del format DD/MM/YYYY
  if (strlen(data) != 10) {
    correcte = 0;
  }

  i = 0;
  while (i < 10) {
    if (i == 2 || i == 5) {
      if (data[i] != '/') {
        correcte = 0;                                                           // Posicions 2 i 5 han de ser '/'
      }
    } else {
      if (data[i] < '0' || data[i] > '9') {
        correcte = 0;                                                           // Altres posicions han de ser dígits
      }
    }
    i = i + 1;
  }

  return correcte;
}

/***********************************************
 * @Finalitat: Analitza una línia de text i extreu la data en format DD/MM/YYYY.
 * @Paràmetres: in: line = cadena amb la data.
 * @Retorn: struct Date amb la data desglossada.
 ***********************************************/
Date parseDateLine(char *line) {
  Date d;
  int i, j;
  char buffer[5];

  // Inicialitzem la data com invàlida
  d.day = -1;
  d.month = -1;
  d.year = -1;

  // Si la línia no comença amb "xx", la processem
  if (strncmp(line, "xx", 2) != 0) {
    i = 0;
    j = 0;

    // Obtenim el dia
    while (line[i] != '/' && line[i] != ' ') {
      buffer[j] = line[i];
      i = i + 1;
      j = j + 1;
    }
    buffer[j] = ' ';
    d.day = atoi(buffer);

    // Obtenim el mes
    i = i + 1;
    j = 0;
    while (line[i] != '/' && line[i] != ' ') {
      buffer[j] = line[i];
      i = i + 1;
      j = j + 1;
    }
    buffer[j] = ' ';
    d.month = atoi(buffer);

    // Obtenim l'any
    i = i + 1;
    j = 0;
    while (line[i] != ' ') {
      buffer[j] = line[i];
      i = i + 1;
      j = j + 1;
    }
    buffer[j] = ' ';
    d.year = atoi(buffer);
  }

  return d;
}

/***********************************************
 * @Finalitat: Valida els arguments d'entrada i analitza la data.
 * @Paràmetres:
 *   in: argc = nombre d'arguments.
 *   in: argv = array de cadenes d'arguments.
 *   out: date = estructura Date amb la data parsejada.
 * @Retorn: 1 si els arguments són vàlids, 0 si no.
 ***********************************************/
int validateArgs(int argc, char *argv[], Date *date) {
  int valid;
  valid = 0;

  // Esperem exactament 3 arguments
  if (argc == 3) {
    // Validem el format de la data
    if (validarFormatData(argv[2])) {
      *date = parseDate(argv[2]);
      if (date->day != -1) {
        valid = 1;
      }
    }
  }

  return valid;
}

/***********************************************
 * @Finalitat: Mostra el menú principal d'opcions.
 * @Paràmetres: ----
 * @Retorn: ----
 ***********************************************/
void mostrarMenu() {
  printf("1. Review inventory | 2. Prepare pack | 3. Book pack | 4. Close\n");
  printf("Select option:\n");
}

/***********************************************
 * @Finalitat: Mostra un missatge de comiat.
 * @Paràmetres: ----
 * @Retorn: ----
 ***********************************************/
void closeProgram() {
  printf("Have a nice day!\n");
}

/***********************************************
 * @Finalitat: Missatge d'error per opció invàlida.
 * @Paràmetres: ----
 * @Retorn: ----
 ***********************************************/
void errorInvalidOption() {
  printf("ERROR: Invalid option.\n");
}

/***********************************************
 * @Finalitat: Missatge d'error per nombre incorrecte d'arguments.
 * @Paràmetres: ----
 * @Retorn: ----
 ***********************************************/
void errorNoArguments() {
  printf("ERROR: Invalid number of arguments.\n");
}

/***********************************************
 * @Finalitat: Missatge d'error per format de data incorrecte.
 * @Paràmetres: ----
 * @Retorn: ----
 ***********************************************/
void errorWrongDateFormat() {
  printf("ERROR: Expected DD/MM/YYYY format for date.\n");
}

/***********************************************
 * @Finalitat: Mostra la talla del casc segons la seva equivalència.
 * @Paràmetres: in: size = talla en format numèric.
 * @Retorn: ----
 ***********************************************/
void displayHelmetSize(float size) {
  int sizeInt = (int)size;

  // Comprovem cada cas i mostrem la talla corresponent
  switch (sizeInt) {
    case 0:
      printf("\tXXS\n");
      break;
    case 1:
      printf("\tXS\n");
      break;
    case 2:
      printf("\tS\n");
      break;
    case 3:
      printf("\tM\n");
      break;
    case 4:
      printf("\tL\n");
      break;
    case 5:
      printf("\tXL\n");
      break;
    case 6:
      printf("\tXXL\n");
      break;
    default:
      // Si no és cap talla coneguda, imprimim el valor en format decimal
      printf("\t%.2f\n", size);
  }
}

/***********************************************
 * @Finalitat: Mostra la informació detallada d’un ítem de l’inventari.
 * @Paràmetres:
 *   in: item = punter a l’ítem.
 *   in: current = data actual de referència.
 * @Retorn: ----
 ***********************************************/
void showItem(Item *item, Date current) {
  int i;

  // Imprimim informació bàsica
  printf("[%c] (%d) %s: %s\n", item->type, item->id, item->brand, item->model);

  // Mostrem si ha estat llogat
  if (item->last_rented.day == -1) {
    printf("Never rented\n");
  } else {
    printf("%02d/%02d/%04d", item->last_rented.day, item->last_rented.month, item->last_rented.year);

    // Marquem amb "!" si la data de lloguer és posterior a l'actual
    if (item->last_rented.year > current.year ||
        (item->last_rented.year == current.year && item->last_rented.month > current.month) ||
        (item->last_rented.year == current.year && item->last_rented.month == current.month &&
         item->last_rented.day > current.day)) {
      printf(" !");
    }
    printf("\n");
  }

  // Preu de lloguer
  printf("Rented for %.2f$\n", item->price);

  // Talles disponibles
  printf("%d available sizes:\n", item->n_sizes);
  for (i = 0; i < item->n_sizes; i++) {
    if (item->type == 'H') {
      displayHelmetSize(item->sizes[i]);                                        // Tractament especial per cascs
    } else {
      printf("\t%.2f\n", item->sizes[i]);
    }
  }
}

/***********************************************
 * @Finalitat: Mostra tot l'inventari i compta quants ítems hi ha de cada tipus.
 * @Paràmetres:
 *   in: items = array de punters a ítems.
 *   in: n = nombre total d’ítems.
 *   in: current = data actual.
 * @Retorn: ----
 ***********************************************/
void showInventory(Item **items, int n, Date current) {
  int i, skis, boots, poles, helmets;

  // Inicialitzem comptadors per tipus
  skis = 0;
  boots = 0;
  poles = 0;
  helmets = 0;

  i = 0;
  while (i < n) {
    showItem(items[i], current);
    printf("\n");

    // Actualitzem comptadors segons el tipus
    if (items[i]->type == 'S') {
      skis = skis + 1;
    } else {
      if (items[i]->type == 'B') {
        boots = boots + 1;
      } else {
        if (items[i]->type == 'P') {
          poles = poles + 1;
        } else {
          if (items[i]->type == 'H') {
            helmets = helmets + 1;
          }
        }
      }
    }

    i = i + 1;
  }

  // Resum final
  printf("Total skis: %d\nTotal boots: %d\nTotal helmets: %d\nTotal poles: %d\n", skis, boots, helmets, poles);
}

/***********************************************
 * @Finalitat: Comprova si un ítem està disponible per llogar.
 * @Paràmetres:
 *   in: item = punter a ítem.
 *   in: current = data actual.
 * @Retorn: 1 si està disponible, 0 si no.
 ***********************************************/
int isAvailable(Item *item, Date current) {
  int available;
  available = 0;

  // Si mai ha estat llogat, està disponible
  if (item->last_rented.day == -1) {
    available = 1;
  } else {
    // Comprovem si la data de retorn ha passat
    if (item->last_rented.year < current.year ||
        (item->last_rented.year == current.year &&
         item->last_rented.month < current.month) ||
        (item->last_rented.year == current.year &&
         item->last_rented.month == current.month &&
         item->last_rented.day <= current.day)) {
      available = 1;
    }
  }

  return available;
}

/***********************************************
 * @Finalitat: Permet preparar un pack d’ítems disponibles i afegir-lo a la llista.
 * @Paràmetres:
 *   in/out: packs = llista enllaçada de packs.
 *   in: items = array d’ítems.
 *   in: n = nombre d’ítems.
 *   in: current = data actual.
 * @Retorn: ----
 ***********************************************/
void preparePack(LinkedList *packs, Item **items, int n, Date current) {
  int i, count, index, disponibles;
  Item **selected;
  float price;
  Element pack;

  disponibles = 0;
  i = 0;

  // Comptem ítems disponibles
  while (i < n) {
    if (isAvailable(items[i], current) == 1) {
      disponibles = disponibles + 1;
    }
    i = i + 1;
  }

  if (disponibles == 0) {
    printf("There are no available items.\n\n");
  } else {
    printf("How many items will the pack contain?\n");
    scanf("%d", &count);

    i = 0;
    int itemNum = 1;

    // Mostrem ítems disponibles amb número identificador
    while (i < n) {
      if (isAvailable(items[i], current) == 1) {
        printf("%d) %s (%s), %.2f$ [%c]\n", itemNum, items[i]->model,
               items[i]->brand, items[i]->price, items[i]->type);
        itemNum++;
      }
      i++;
    }

    selected = (Item **)malloc(sizeof(Item *) * count);
    if (selected != NULL) {
      i = 0;

      // Seleccionem ítems pel pack segons número mostrat
      while (i < count) {
        printf("Pack item #%d: ", i + 1);
        scanf("%d", &index);

        int availableIndex = 0;
        int j = 0;
        int trobat = 0;

        while (j < n && trobat == 0) {
          if (isAvailable(items[j], current) == 1) {
            availableIndex++;
            if (availableIndex == index) {
              selected[i] = items[j];
              trobat = 1;
            }
          }
          j++;
        }

        i++;
      }

      // Assignem preu i afegim a la llista de packs
      printf("Pack price: ");
      scanf("%f", &price);
      pack = LINKEDLIST_createElement(selected, count, price);
      LINKEDLIST_add(packs, pack, LINKEDLIST_size(*packs));
      printf("\nPack added successfully!\n\n");
    } else {
      printf("Memory error.\n\n");
    }
  }
}

/***********************************************
 * @Finalitat: Permet reservar un pack (assignar data de lloguer) i eliminar-lo de la llista.
 * @Paràmetres:
 *   in/out: packs = llista de packs.
 *   in: current = data actual.
 * @Retorn: ----
 ***********************************************/
void bookPack(LinkedList *packs, Date current) {
  int i, choice, size;
  Element e;

  size = LINKEDLIST_size(*packs);
  if (size == 0) {
    printf("There are no available packs.\n\n");
  } else {
    i = 0;

    // Mostrem cada pack
    while (i < size) {
      e = LINKEDLIST_get(packs, i);
      printf("\t%d. Pack #%d (%d items) for %.2f$:\n", i + 1, i + 1, e.n_items, e.price);
      int j = 0;
      while (j < e.n_items) {
        printf("\t\t[%c] %s (%s)\n", e.items[j]->type, e.items[j]->model, e.items[j]->brand);
        j = j + 1;
      }
      i = i + 1;
    }

    printf("\t%d. Back\n", size + 1);
    printf("\tSelect option:");
    scanf("%d", &choice);
    choice = choice - 1; // Índex base 0

    if (choice >= 0 && choice < size) {
      e = LINKEDLIST_get(packs, choice);
      i = 0;

      // Assignem nova data de lloguer (un any després)
      while (i < e.n_items) {
        e.items[i]->last_rented.day = current.day;
        e.items[i]->last_rented.month = current.month;
        e.items[i]->last_rented.year = current.year + 1;
        i = i + 1;
      }

      LINKEDLIST_remove(packs, choice);
      printf("\nPack successfully booked!\n\n");
    } else if (choice == size) {
      printf("\n"); // Opció de tornar enrere
    } else {
      printf("Invalid selection.\n\n");
    }
  }
}

/***********************************************
 * @Finalitat: Carrega l'inventari des d’un fitxer de text.
 * @Paràmetres:
 *   in: filename = nom del fitxer.
 *   out: items = array d’ítems que es va omplint.
 *   in/out: n = nombre total d’ítems.
 * @Retorn: ----
 ***********************************************/
void loadInventory(char *filename, Item **items, int *n) {
  FILE *f;
  char line[MAX_LINE];
  int i, id, n_sizes, index;
  char type;
  float price;
  Item *item;
  char *sizes_line;

  f = fopen(filename, "r");
  if (f != NULL) {
    fgets(line, MAX_LINE, f);                                                   // Saltar capçalera

    while (fgets(line, MAX_LINE, f)) {
      i = 0;
      while (line[i] != ' ') {
        i++;
      }
      line[i] = '\0';
      id = atoi(line);

      while (line[i] != '(') {
        i++;
      }
      i++;
      type = line[i];

      while (line[i] != '-') {
        i++;
      }
      i++;
      price = atof(&line[i]);

      item = (Item *)malloc(sizeof(Item));
      if (item != NULL) {
        item->id = id;
        item->type = type;
        item->price = price;

        // Marca
        if (fgets(line, MAX_LINE, f) != NULL) {
          line[strcspn(line, "\n")] = '\0';
          item->brand = (char *)malloc(strlen(line) + 1);

          if (item->brand != NULL) {
            strcpy(item->brand, line);

            // Model
            if (fgets(line, MAX_LINE, f) != NULL) {
              line[strcspn(line, "\n")] = '\0';
              item->model = (char *)malloc(strlen(line) + 1);

              if (item->model != NULL) {
                strcpy(item->model, line);

                // Talles
                if (fgets(line, MAX_LINE, f) != NULL) {
                  i = 0;
                  while (line[i] != ':') {
                    i++;
                  }
                  line[i] = '\0';
                  n_sizes = atoi(line);
                  item->n_sizes = n_sizes;

                  item->sizes = (float *)malloc(sizeof(float) * n_sizes);
                  if (item->sizes != NULL) {
                    sizes_line = &line[i + 1];
                    index = 0;
                    i = 0;
                    while (i < n_sizes && sizes_line[index] != '\0') {
                      while (sizes_line[index] == ' ') {
                        index++;
                      }
                      item->sizes[i] = atof(&sizes_line[index]);
                      while (sizes_line[index] != ' ' && sizes_line[index] != '\0' && sizes_line[index] != '\n') {
                        index++;
                      }
                      i++;
                    }

                    // Última línia: data
                    if (fgets(line, MAX_LINE, f) != NULL) {
                      line[strcspn(line, "\n")] = '\0';
                      item->last_rented = parseDateLine(line);

                      items[*n] = item;
                      *n = *n + 1;
                    } else {
                      // Alliberem si hi ha error
                      free(item->sizes);
                      free(item->model);
                      free(item->brand);
                      free(item);
                    }
                  } else {
                    free(item->model);
                    free(item->brand);
                    free(item);
                    printf("Memory allocation error\n");
                  }
                } else {
                  free(item->model);
                  free(item->brand);
                  free(item);
                }
              } else {
                free(item->brand);
                free(item);
                printf("Memory allocation error\n");
              }
            } else {
              free(item->brand);
              free(item);
            }
          } else {
            free(item);
            printf("Memory allocation error\n");
          }
        } else {
          free(item);
        }
      } else {
        printf("Memory allocation error\n");
      }
    }
    fclose(f);
  }
}

/***********************************************
 * @Finalitat: Allibera tota la memòria reservada pels ítems.
 * @Paràmetres:
 *   in: items = array de punters a ítems.
 *   in: n = nombre total d’ítems.
 * @Retorn: ----
 ***********************************************/
void freeInventory(Item **items, int n) {
  int i;
  for (i = 0; i < n; i++) {
    if (items[i] != NULL) {
      if (items[i]->brand != NULL) {
        free(items[i]->brand);
      }
      if (items[i]->model != NULL) {
        free(items[i]->model);
      }
      if (items[i]->sizes != NULL) {
        free(items[i]->sizes);
      }
      free(items[i]);
    }
  }
}

/***********************************************
 * @Finalitat: Procediment principal del programa LS Ski.
 * Controla el flux general d'execució.
 * @Paràmetres:
 *   in: argc = nombre d’arguments.
 *   in: argv = array d’arguments (fitxer, data).
 * @Retorn: 0 (final correcte).
 ***********************************************/
int main(int argc, char *argv[]) {
  Item *items[MAX_ITEMS];                                                       // Array per guardar punters a ítems
  int n_items;                                                                  // Comptador d'ítems a l'inventari
  LinkedList packs;                                                             // Llista de packs preparats
  int option;                                                                   // Opció escollida pel menú
  Date current;                                                                 // Data actual introduïda per l’usuari

  n_items = 0;
  packs = LINKEDLIST_create();

  // Comprovem arguments
  if (argc != 3) {
    errorNoArguments();
  } else {
    if (!validarFormatData(argv[2])) {
      errorWrongDateFormat();
    } else {
      current = parseDate(argv[2]);

      // Comprovem que la data sigui vàlida
      if (current.day == -1 || current.month < 1 || current.month > 12 || current.year == -1) {
        errorWrongDateFormat();
      } else {
        loadInventory(argv[1], items, &n_items);
        printf("Welcome to LS Ski!\n\n");

        option = 0;
        while (option != 4) {
          mostrarMenu();
          scanf("%d", &option);

          if (option == 1) {
            showInventory(items, n_items, current);
            printf("\n");
          } else {
            if (option == 2) {
              preparePack(&packs, items, n_items, current);
            } else {
              if (option == 3) {
                bookPack(&packs, current);
              } else {
                if (option != 4) {
                  errorInvalidOption();
                  printf("\n");
                }
              }
            }
          }
        }

        closeProgram();

        // Alliberem la memòria al final del programa
        freeInventory(items, n_items);
        LINKEDLIST_destroy(&packs);
      }
    }
  }

  return 0;
}