/***********************************************
 * @Propòsit: Realitzar la pràctica 1.2
 * @Autor/s: Judith Caralt Rico
 * @Data de creació: 20/04/25
 * @Data de l’última modificació: 19/05/25
 *************************************************/

#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  

/* Constantes del sistema */
#define MAX_DOCTORS 30                                                          /* Número máximo de doctores permitidos */
#define MAX_HOSPITAL_VISITS 50                                                  /* Número máximo de visitas permitidas */

/* 
 * Estructura para almacenar datos de un doctor
 * Cada doctor tiene un ID único de 6 dígitos y un mes de vacaciones
 * Los doctores no pueden atender visitas durante su mes de vacaciones
 */
typedef struct {
  char id[7];                                                                   /* ID del doctor: 6 dígitos + terminador nulo '\0' */
  int mesVacaciones;                                                            /* Mes de vacaciones (1-12) donde el doctor no está disponible */
} Doctor;

/* 
 * Estructura para almacenar datos de una visita médica
 * Cada visita asocia un paciente con un doctor en una fecha específica
 */
typedef struct {
  char patientName[50];                                                         /* Nombre del paciente que solicita la visita */
  char doctorID[7];                                                             /* ID del doctor que atenderá la visita */
  int day;                                                                      /* Día del mes (1-30) para la visita */
  char month[10];                                                               /* Nombre del mes (en texto) para la visita */
} Visit;

/*
 * Convierte una cadena de caracteres en un número entero
 * Implementación propia de atoi para números pequeños
 * 
 * Esta función realiza la conversión caracter a caracter, acumulando
 * el valor numérico. Para cada dígito, multiplica el valor acumulado por 10
 * y suma el valor numérico del dígito actual (restando el código ASCII de '0').
 * 
 * Parámetros:
 *    - aux: Cadena a convertir (generalmente con 2 dígitos para meses/días)
 * Retorna:
 *    - El valor numérico convertido (por ejemplo, "06" a 6, "12" a 12)
 */
 
int getNumberAtoi(char aux[3]) {
  int n = 0;                                                                    /* Acumulador para el valor numérico resultante */
  int i = 0;                                                                    /* Variable de control para el bucle */
  int length = strlen(aux);                                                     /* Longitud de la cadena a convertir */
  
  /* Recorre cada carácter de la cadena y lo convierte a su valor numérico */
  for (i = 0; i < length; i++) {                                          
    n = n * 10;                                                                 /* Multiplica por 10 para mover el dígito a la izquierda */
    n = n + aux[i] - '0';                                                       /* Suma el valor numérico del dígito actual */
  }
  return n;                                                                     /* Devuelve el valor convertido */
}

/*
 * Añade un nuevo doctor al sistema
 * Solicita al usuario un ID de 6 dígitos y un mes de vacaciones
 * Parámetros:
 *    - doctors: Array de doctores donde se añadirá el nuevo doctor
 */
void addDoctor(Doctor doctors[]) {
  int contadorDoctores = 0;
  char entrada[100];
  char idDoctor[7];
  char mesTexto[3];
  int mesNumerico;
  int posicionGuion;
  int conteoGuiones;
  int valido;
  int existe;
  int i, j;
  char dummy[256];
  int len;

  /* Contar cuántos doctores hay en el sistema */
  for (i = 0; i < MAX_DOCTORS; i++) {
    if (doctors[i].id[0] != '\0') {
      contadorDoctores++;
    }
  }

  /* Verificar si se ha alcanzado el límite de doctores */
  if (contadorDoctores >= MAX_DOCTORS) {
    printf("Maximum number of doctors reached.\n");

  } else {
    valido = 0;

    /* Bucle para solicitar y validar los datos del doctor */
    while (!valido) {
      printf("Enter ID and vacation month (ex. 123456-06): ");
      fgets(entrada, sizeof(entrada), stdin);
      
      /* Eliminar el salto de línea si existe */
      len = strlen(entrada);
      if (len > 0 && entrada[len - 1] == '\n') {
        entrada[len - 1] = '\0';
      } else {
        /* Limpiar el buffer de entrada */
        char dummy[256];
        fgets(dummy, sizeof(dummy), stdin);
      }

      /* Buscar el guión y contar cuántos hay */
      posicionGuion = -1;
      conteoGuiones = 0;
      for (i = 0; entrada[i] != '\0'; i++) {
        if (entrada[i] == '-') {
          if (conteoGuiones == 0) {
            posicionGuion = i;
          }
          conteoGuiones++;
        }
      }

      /* Validar el formato de entrada */
      if (conteoGuiones != 1 || posicionGuion == 0 || posicionGuion == (int)strlen(entrada) - 1) {
        /* Error: formato incorrecto. Debe tener exactamente un guion en medio */
        printf("ERROR: Wrong format. Should be <ID>-<month>.\n");
      } else {
        /* Extraer el ID del doctor de la entrada */
        for (i = 0; i < posicionGuion && i < 6; i++) {
          idDoctor[i] = entrada[i];
        }
        idDoctor[i] = '\0';                                                     /* Añadir terminador nulo para formar una cadena válida */

        /* Extraer el mes de vacaciones de la entrada */
        j = 0;
        for (i = posicionGuion + 1; entrada[i] != '\0' && j < 2; i++) {
          mesTexto[j] = entrada[i];
          j++;
        }
        mesTexto[j] = '\0';                                                     /* Añadir terminador nulo para formar una cadena válida */

        /* Validar longitud del ID: debe tener exactamente 6 dígitos */
        if (strlen(idDoctor) != 6) {
          printf("ERROR: ID must be 6 digits.\n");
        } else {
          /* Validar que el ID no comience con 0 */
          if (idDoctor[0] == '0') {
            printf("ERROR: ID cannot begin with 0.\n");
          } else {
            /* Convertir y validar el formato y valor del mes */
            mesNumerico = getNumberAtoi(mesTexto);
            
            /* Validar que el mes tenga formato de dos dígitos */
            if (strlen(mesTexto) != 2) {
              printf("ERROR: Wrong month format. Please use two digits.\n");
            } else {
              /* Validar que el valor del mes esté entre 1 y 12 */
              if (mesNumerico < 1 || mesNumerico > 12) {
                printf("ERROR: Wrong month value. It must be between 01 and 12.\n");
              } else {
                /* Verificar si el doctor ya existe en el sistema */
                existe = 0;
                for (i = 0; i < MAX_DOCTORS; i++) {
                  if (strcmp(doctors[i].id, idDoctor) == 0) {
                    existe = 1;                                                 /* Doctor encontrado, marcar como existente */
                  }
                }

                if (existe) {
                  /* No se puede añadir el doctor porque ya existe uno con el mismo ID */
                  printf("Failed to add doctor: Doctor already exists.\n");
                  valido = 1;                                                   /* Entrada válida (aunque no se añada el doctor) */
                } else {
                  /* Buscar una posición libre en el array para añadir el nuevo doctor */
                  for (i = 0; i < MAX_DOCTORS; i++) {
                    if (doctors[i].id[0] == '\0' && valido == 0) {
                      /* Posición libre encontrada, guardar los datos del doctor */
                      strcpy(doctors[i].id, idDoctor);
                      doctors[i].mesVacaciones = mesNumerico;
                      printf("Doctor added successfully.\n");
                      valido = 1;                                               /* Marcar como procesado para salir del bucle */
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

/*
 * Muestra todos los doctores registrados en el sistema
 * Parámetros:
 *    - doctors: Array de doctores a listar
 */
void listDoctors(Doctor doctors[]) {
  int i;
  int doctorsFound = 0;
  int doctorNumber;
  /* Contar cuántos doctores hay en el sistema */
  for (i = 0; i < MAX_DOCTORS; i++) {
    if (doctors[i].id[0] != '\0') {
      doctorsFound++;
    }
  }

  /* Mostrar mensaje si no hay doctores */
  if (doctorsFound == 0) {
    printf("No doctors exist in the system yet.\n");
  } else {
    /* Mostrar la lista de doctores */
    printf("Doctors List (total of %d):\n", doctorsFound);

    doctorNumber = 1;
    for (i = 0; i < MAX_DOCTORS; i++) {
      if (doctors[i].id[0] != '\0') {
        printf("\t%d) Doctor ID: %s, Vacation month: ", doctorNumber,
               doctors[i].id);
        /* Mostrar el nombre del mes basado en su número */
        switch (doctors[i].mesVacaciones) {
        case 1:
          printf("January\n");                                                  /* Enero (mes 1) */
          break;
        case 2:
          printf("February\n");                                                 /* Febrero (mes 2) */
          break;
        case 3:
          printf("March\n");                                                    /* Marzo (mes 3) */
          break;
        case 4:
          printf("April\n");                                                    /* Abril (mes 4) */
          break;
        case 5:
          printf("May\n");                                                      /* Mayo (mes 5) */
          break;
        case 6:
          printf("June\n");                                                     /* Junio (mes 6) */
          break;
        case 7:
          printf("July\n");                                                     /* Julio (mes 7) */
          break;
        case 8:
          printf("August\n");                                                   /* Agosto (mes 8) */
          break;
        case 9:
          printf("September\n");                                                /* Septiembre (mes 9) */
          break;
        case 10:
          printf("October\n");                                                  /* Octubre (mes 10) */
          break;
        case 11:
          printf("November\n");                                                 /* Noviembre (mes 11) */
          break;
        case 12:
          printf("December\n");                                                 /* Diciembre (mes 12) */
          break;
        default:
          printf("Unknown\n");                                                  /* Valor de mes no válido */
        }
        doctorNumber++;
      }
    }
  }
}

/*
 * Elimina un doctor del sistema y todas sus visitas asociadas
 * Parámetros:
 *    - doctors: Array de doctores
 *    - visits: Array de visitas
 */
void removeDoctor(Doctor doctors[], Visit visits[]) {
  char idDoctor[7];
  int i, j, valid = 0, found = 0;
  char dummy[256];
  int len;

  /* Comprobar si hay doctores en el sistema */
  if (doctors[0].id[0] == '\0') {
    printf("No doctors exist in the system yet.\n");
  } else {
    /* Solicitar y validar el ID del doctor a eliminar */
    while (!valid) {
      printf("Enter the ID of the doctor to remove: ");
      fgets(idDoctor, sizeof(idDoctor), stdin);

      /* Eliminar el salto de línea si existe */
      len = strlen(idDoctor);
      if (len > 0 && idDoctor[len - 1] == '\n') {
        idDoctor[len - 1] = '\0';
        len--;
      } else {
        /* Limpiar el buffer de entrada */
        fgets(dummy, sizeof(dummy), stdin);
      }

      /* Validar el ID */
      if (idDoctor[0] == '0') {
        printf("ERROR: ID cannot begin with 0.\n");
      } else {
        if (len != 6) {
          printf("ERROR: ID must be 6 digits.\n");
        } else {
          valid = 1;
        }
      }
    }

    /* Buscar el doctor en el sistema */
    i = 0;
    found = 0;
    while (i < MAX_DOCTORS && !found) {
      if (strcmp(doctors[i].id, idDoctor) == 0) {
        /* Eliminar todas las visitas asociadas a este doctor */
        for (j = 0; j < MAX_HOSPITAL_VISITS; j++) {
          if (strcmp(visits[j].doctorID, idDoctor) == 0) {
            visits[j].patientName[0] = '\0';
            visits[j].doctorID[0] = '\0';
            visits[j].day = 0;
            visits[j].month[0] = '\0';
          }
        }

        /* Eliminar el doctor */
        doctors[i].id[0] = '\0';
        doctors[i].mesVacaciones = 0;
        printf("Doctor removed successfully.\n");
        found = 1;
      }
      i++;
    }

    /* Mostrar mensaje si no se encontró el doctor */
    if (!found) {
      printf("Failed to remove doctor: Doctor does not exist.\n");
    }
  }
}

/*
 * Añade una nueva visita médica al sistema
 * Solicita nombre del paciente, ID del doctor, mes y día de la visita
 * Parámetros:
 *    - visits: Array de visitas donde se añadirá la nueva visita
 *    - doctors: Array de doctores para validar la disponibilidad
 */
void addVisit(Visit visits[], Doctor doctors[]) {
  int totalDoctors = 0;
  int totalVisits = 0;
  int i, j;
  char entrada[100];
  char idDoctor[7];
  char mesTexto[3];
  char diaTexto[3];
  int mesNumerico, diaNumerico;
  int guion1, guion2;
  int totalGuiones;
  int valido = 0;
  int procesarDatos = 1;
  char patientName[50];
  char dummy[256];
  int doctorExists;
  int doctorIndex;
  int visitAdded;
  int len;
  int errorFound;

  /* Contar el total de visitas en el sistema */
  for (i = 0; i < MAX_HOSPITAL_VISITS; i++) {
    if (visits[i].patientName[0] != '\0') {
      totalVisits++;
    }
  }

  /* Verificar si se ha alcanzado el límite de visitas */
  if (totalVisits >= MAX_HOSPITAL_VISITS) {
    printf("Maximum number of visits reached.\n");
    procesarDatos = 0;
  }

  /* Contar el total de doctores en el sistema */
  if (procesarDatos) {
    for (i = 0; i < MAX_DOCTORS; i++) {
      if (doctors[i].id[0] != '\0') {
        totalDoctors++;
      }
    }

    /* Verificar si hay doctores en el sistema */
    if (totalDoctors == 0) {
      printf("No doctors exist in the system yet.\n");
      procesarDatos = 0;
    }
  }

  /* Solicitar y validar los datos del paciente */
  if (procesarDatos) {
    valido = 0;
    while (!valido) {
      printf("Enter your name: ");
      fgets(patientName, sizeof(patientName), stdin);

      /* Eliminar el salto de línea si existe */
      len = strlen(patientName);
      if (len > 0 && patientName[len - 1] == '\n') {
        patientName[len - 1] = '\0';
        len--;
      } else {
        /* Limpiar el buffer de entrada */
        fgets(dummy, sizeof(dummy), stdin);
      }

      valido = 1;

      /* Validar el nombre del paciente */
      if (len == 0) {
        printf("ERROR: Name cannot be empty.\n");
        valido = 0;
      } else {
        errorFound = 0;
        for (i = 0; i < len && !errorFound; i++) {
          if ((patientName[i] < 'A' || patientName[i] > 'Z') &&
              (patientName[i] < 'a' || patientName[i] > 'z') &&
              patientName[i] != ' ') {
            printf("ERROR: Name can only contain letters.\n");
            valido = 0;
            errorFound = 1;
          }
        }
      }
    }
  }

  /* Solicitar y validar los datos de la visita */
  if (procesarDatos) {
    valido = 0;
    while (!valido && procesarDatos) {
      printf("Enter doctor ID, month and day of visit (ex. 123456-06-27): ");
      fgets(entrada, sizeof(entrada), stdin);

      /* Eliminar el salto de línea si existe */
      len = strlen(entrada);
      if (len > 0 && entrada[len - 1] == '\n') {
        entrada[len - 1] = '\0';
      } else {
        /* Limpiar el buffer de entrada */
        fgets(dummy, sizeof(dummy), stdin);
      }

      /* Buscar los guiones y contar cuántos hay */
      guion1 = -1;
      guion2 = -1;
      totalGuiones = 0;
      for (i = 0; entrada[i] != '\0'; i++) {
        if (entrada[i] == '-') {
          if (totalGuiones == 0) {
            guion1 = i;
          }
          if (totalGuiones == 1) {
            guion2 = i;
          }
          totalGuiones++;
        }
      }

      /* Validar el formato de entrada */
      if (totalGuiones != 2 || guion2 == (int)strlen(entrada) - 1 ||
          guion1 == (int)strlen(entrada) - 1) {
        printf("ERROR: Wrong format. Should be <ID>-<month>-<day>.\n");
      } else {
        
        /* Extraer el ID del doctor */
        for (i = 0; i < guion1 && i < 6; i++) {
          idDoctor[i] = entrada[i];
        }
        idDoctor[i] = '\0';

        /* Extraer el mes */
        j = 0;
        for (i = guion1 + 1; i < guion2 && j < 2; i++) {
          mesTexto[j] = entrada[i];
          j++;
        }
        mesTexto[j] = '\0';

        /* Extraer el día */
        j = 0;
        for (i = guion2 + 1; entrada[i] != '\0' && j < 2; i++) {
          diaTexto[j] = entrada[i];
          j++;
        }
        diaTexto[j] = '\0';

        /* Validar el ID del doctor */
        if (strlen(idDoctor) != 6) {
          printf("ERROR: ID must be 6 digits.\n");
        } else {
          if (idDoctor[0] == '0') {
            printf("ERROR: ID cannot begin with 0.\n");
          } else {
            /* Convertir y validar mes y día */
            mesNumerico = getNumberAtoi(mesTexto);
            diaNumerico = getNumberAtoi(diaTexto);

            if (strlen(mesTexto) != 2 || mesNumerico == 0) {
              printf("ERROR: Wrong month format. Please use two digits.\n");
            } else {
              if (mesNumerico < 1 || mesNumerico > 12) {
                printf("ERROR: Wrong month value. It must be between 01 and 12.\n");
              } else {
                if (strlen(diaTexto) != 2 || diaNumerico == 0) {
                  printf("ERROR: Wrong day format. Please use two digits.\n");
                } else {
                  if (diaNumerico < 1 || diaNumerico > 30) {
                    printf("ERROR: Wrong day value. It must be between 01 and 30.\n");
                  } else {
                    /* Verificar si el doctor existe */
                    doctorExists = 0;
                    doctorIndex = -1;

                    for (i = 0; i < MAX_DOCTORS; i++) {
                      if (strcmp(doctors[i].id, idDoctor) == 0) {
                        doctorExists = 1;
                        doctorIndex = i;
                        i = MAX_DOCTORS;                                        
                      }
                    }

                    if (!doctorExists) {
                      printf("Doctor %s not found.\n", idDoctor);
                      procesarDatos = 0;
                      valido = 1;
                    } else {
                      /* Verificar si el doctor está de vacaciones ese mes */
                      if (doctors[doctorIndex].mesVacaciones == mesNumerico) {
                        printf("Doctor is on vacation in ");
                        /* Mostrar el nombre del mes */
                        switch (mesNumerico) {
                          case 1: printf("January.\n"); break;
                          case 2: printf("February.\n"); break;
                          case 3: printf("March.\n"); break;
                          case 4: printf("April.\n"); break;
                          case 5: printf("May.\n"); break;
                          case 6: printf("June.\n"); break;
                          case 7: printf("July.\n"); break;
                          case 8: printf("August.\n"); break;
                          case 9: printf("September.\n"); break;
                          case 10: printf("October.\n"); break;
                          case 11: printf("November.\n"); break;
                          case 12: printf("December.\n"); break;
                        }
                        procesarDatos = 0;
                        valido = 1; // Stop the loop
                      } else {
                        /* Añadir la visita al sistema */
                        visitAdded = 0;
                        for (i = 0; i < MAX_HOSPITAL_VISITS; i++) {
                          if (visits[i].patientName[0] == '\0' && !visitAdded) {
                            strcpy(visits[i].patientName, patientName);
                            strcpy(visits[i].doctorID, idDoctor);
                            visits[i].day = diaNumerico;

                            /* Asignar el nombre del mes según el número */
                            switch(mesNumerico) {
                              case 1: strcpy(visits[i].month, "January"); break;
                              case 2: strcpy(visits[i].month, "February"); break;
                              case 3: strcpy(visits[i].month, "March"); break;
                              case 4: strcpy(visits[i].month, "April"); break;
                              case 5: strcpy(visits[i].month, "May"); break;
                              case 6: strcpy(visits[i].month, "June"); break;
                              case 7: strcpy(visits[i].month, "July"); break;
                              case 8: strcpy(visits[i].month, "August"); break;
                              case 9: strcpy(visits[i].month, "September"); break;
                              case 10: strcpy(visits[i].month, "October"); break;
                              case 11: strcpy(visits[i].month, "November"); break;
                              case 12: strcpy(visits[i].month, "December"); break;
                            }

                            printf("Visit added successfully.\n");
                            visitAdded = 1;
                            valido = 1;
                            i = MAX_HOSPITAL_VISITS; 
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

/*
 * Muestra las visitas médicas de un paciente específico
 * Parámetros:
 *    - visits: Array de visitas a consultar
 */
void listVisits(Visit visits[]) {
  int i, valid = 0;
  char patientName[50];
  int visitsFound = 0;
  char dummy[256];
  int len;
  int errorFound;

  /* Solicitar y validar el nombre del paciente */
  while (!valid) {
    printf("Enter your name: ");
    fgets(patientName, sizeof(patientName), stdin);

    /* Eliminar el salto de línea si existe */
    len = strlen(patientName);
    if (len > 0 && patientName[len - 1] == '\n') {
      patientName[len - 1] = '\0';
      len--;
    } else {
      /* Limpiar el buffer de entrada */
      fgets(dummy, sizeof(dummy), stdin);
    }
    
    valid = 1;
    /* Validar el nombre del paciente */
    if (len == 0) {
      printf("ERROR: Name cannot be empty.\n");
      valid = 0;
    } else {
      errorFound = 0;
      for (i = 0; i < len && !errorFound; i++) {
        if ((patientName[i] < 'A' || patientName[i] > 'Z') &&
            (patientName[i] < 'a' || patientName[i] > 'z') &&
            patientName[i] != ' ') {
          printf("ERROR: Name can only contain letters.\n");
          valid = 0;
          errorFound = 1;
        }
      }
    }
  }

  /* Contar cuántas visitas tiene el paciente */
  for (i = 0; i < MAX_HOSPITAL_VISITS; i++) {
    if (visits[i].patientName[0] != '\0' && strcmp(visits[i].patientName, patientName) == 0) {
      visitsFound++;
    }
  }

  /* Mostrar las visitas del paciente */
  if (visitsFound == 0) {
    printf("No visits have been found for patient %s.\n", patientName);
  } else {
    printf("%d visit(s) found for patient %s:\n", visitsFound, patientName);
    int visitIndex = 1;
    for (i = 0; i < MAX_HOSPITAL_VISITS; i++) {
      if (visits[i].patientName[0] != '\0' && strcmp(visits[i].patientName, patientName) == 0) {
        printf("\t%d. Doctor ID: %s --- Date: %d of %s\n", visitIndex,
               visits[i].doctorID, visits[i].day, visits[i].month);
        visitIndex++;
      }
    }
  }
}

/*
 * Cancela una visita médica de un paciente específico
 * Parámetros:
 *    - visits: Array de visitas
 */
void cancelVisit(Visit visits[]) {
  int i, j, valid = 0, found = 0;
  char patientName[50];
  char entrada[100];
  int visitNumber;
  int visitsFound = 0;
  int visitIndex[MAX_HOSPITAL_VISITS];
  char dummy[256];
  int len;
  int errorFound;

  /* Solicitar y validar el nombre del paciente */
  while (!valid) {
    printf("Enter your name: ");
    fgets(patientName, sizeof(patientName), stdin);

    /* Eliminar el salto de línea si existe */
    len = strlen(patientName);
    if (len > 0 && patientName[len - 1] == '\n') {
      patientName[len - 1] = '\0';
      len--;
    } else {
      /* Limpiar el buffer de entrada */
      fgets(dummy, sizeof(dummy), stdin);
    }

    valid = 1;
    /* Validar el nombre del paciente */
    if (len == 0) {
      printf("ERROR: Name cannot be empty.\n");
      valid = 0;
    } else {
      errorFound = 0;
      for (i = 0; i < len && !errorFound; i++) {
        if ((patientName[i] < 'A' || patientName[i] > 'Z') &&
            (patientName[i] < 'a' || patientName[i] > 'z') &&
            patientName[i] != ' ') {
          printf("ERROR: Name can only contain letters.\n");
          valid = 0;
          errorFound = 1;
        }
      }
    }
  }

  /* Buscar las visitas del paciente */
  for (i = 0; i < MAX_HOSPITAL_VISITS; i++) {
    if (visits[i].patientName[0] != '\0' && strcmp(visits[i].patientName, patientName) == 0) {
      visitIndex[visitsFound] = i;
      visitsFound++;
    }
  }

  /* Mostrar las visitas del paciente */
  if (visitsFound == 0) {
    printf("No visits have been found for patient %s.\n", patientName);
  } else {
    printf("%d visit(s) found for patient %s:\n", visitsFound, patientName);
    for (i = 0; i < visitsFound; i++) {
      printf("\t%d. Doctor ID: %s --- Date: %d of %s\n", i + 1,
             visits[visitIndex[i]].doctorID, visits[visitIndex[i]].day, visits[visitIndex[i]].month);
    }
    printf("\t%d. Back\n\n", visitsFound + 1);

    /* Solicitar y validar la visita a cancelar */
    valid = 0;
    while (!valid) {
      printf("Enter the number of the visit to cancel: ");
      fgets(entrada, sizeof(entrada), stdin);

      /* Eliminar el salto de línea si existe */
      len = strlen(entrada);
      if (len > 0 && entrada[len - 1] == '\n') {
        entrada[len - 1] = '\0';
      } else {
        /* Limpiar el buffer de entrada */
        fgets(dummy, sizeof(dummy), stdin);
      }

      visitNumber = getNumberAtoi(entrada);

      /* Validar el número de visita */
      if (visitNumber < 1 || visitNumber > visitsFound + 1) {
        printf("ERROR: Invalid visit number.\n");
      } else {
        if (visitNumber == visitsFound + 1) {
          /* Volver atrás sin cancelar */
          valid = 1;
        } else {
          /* Cancelar la visita seleccionada */
          visits[visitIndex[visitNumber - 1]].patientName[0] = '\0';
          visits[visitIndex[visitNumber - 1]].doctorID[0] = '\0';
          visits[visitIndex[visitNumber - 1]].day = 0;
          visits[visitIndex[visitNumber - 1]].month[0] = '\0';
          printf("Visit cancelled successfully.\n");
          valid = 1;
        }
      }
    }
  }
}

/*
 * Función principal que gestiona el menú y la ejecución del programa
 * Muestra un prompt y procesa los comandos ingresados por el usuario
 * Este sistema permite gestionar doctores y visitas médicas en un hospital
 * A través de un menú de comandos de texto interactivo
 */
int main() {
  /* Buffer para almacenar el comando ingresado por el usuario */
  char command[100];
  /* Flag para controlar la salida del bucle principal */
  int exit = 0;
  /* Arrays para almacenar la información de doctores y visitas */
  Doctor doctors[MAX_DOCTORS];
  Visit visits[MAX_HOSPITAL_VISITS];
  /* Variables auxiliares para control de bucles y procesamiento */
  int i;
  char dummy[256];                                                              /* Buffer para limpiar la entrada estándar */
  int len;

  /* Inicializar el array de doctores: marcar todos como no utilizados */
  for(i = 0; i < MAX_DOCTORS; i++) {
    doctors[i].id[0] = '\0'; /* Un id que comienza con \0 indica posición libre */
    doctors[i].mesVacaciones = 0;
  }

  /* Inicializar el array de visitas: marcar todas como no utilizadas */
  for(i = 0; i < MAX_HOSPITAL_VISITS; i++) {
    visits[i].patientName[0] = '\0';                                            /* Nombre vacío indica visita no usada */
    visits[i].doctorID[0] = '\0';
    visits[i].day = 0;
    visits[i].month[0] = '\0';
  }
   
  /* Mostrar el mensaje de bienvenida con formato */
  printf("-----------------------------------------------------------------\n");
  printf("\t\tWELCOME TO LSHEALTH MANAGER\n");
  printf("-----------------------------------------------------------------\n");

  /* Bucle principal del programa: se ejecuta hasta que el usuario ingresa "exit" */
  while (!exit) {
    /* Mostrar el prompt y obtener el comando del usuario */
    printf("admin@LSHealth> ");
    fgets(command, sizeof(command), stdin);

    /* Eliminar el salto de línea final del comando si existe */
    len = strlen(command);
    if (len > 0 && command[len - 1] == '\n') {
      command[len - 1] = '\0';                                                  /* Reemplazar \n con \0 para terminar cadena */
    } else {
      /* Si no hay \n, limpiar el buffer de entrada para evitar problemas */
      fgets(dummy, sizeof(dummy), stdin);
    }

    /* Procesar el comando ingresado comparando con las opciones disponibles */
    if (strcmp(command, "add doctor") == 0) {
      /* Comando para añadir un nuevo doctor al sistema */
      addDoctor(doctors);
    } else {
      if (strcmp(command, "remove doctor") == 0) {
        /* Comando para eliminar un doctor existente y sus visitas asociadas */
        removeDoctor(doctors, visits);
      } else {
        if (strcmp(command, "list doctors") == 0) {
          /* Comando para mostrar la lista de todos los doctores registrados */
          listDoctors(doctors);
        } else {
          if (strcmp(command, "add visit") == 0) {
            /* Comando para programar una nueva visita médica */
            addVisit(visits, doctors);
          } else {
            if (strcmp(command, "cancel visit") == 0) {
              /* Comando para cancelar una visita médica existente */
              cancelVisit(visits);
            } else {
              if (strcmp(command, "list visits") == 0) {
                /* Comando para listar las visitas de un paciente específico */
                listVisits(visits);
              } else {
                if (strcmp(command, "exit") == 0) {
                  /* Comando para salir del programa */
                  exit = 1;                                                     /* Establecer flag de salida para terminar el bucle */
                } else {
                  /* Si el comando no coincide con ninguna opción, mostrar error */
                  printf("ERROR: Command not found.\n");
                }
              }
            }
          }
        }
      }
    }
  }
  return 0; 
}