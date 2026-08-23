/***********************************************
 * @Propòsit: Realitzar la pràctica 2.1
 * @Autor/s: Judith Caralt Rico
 * @Data de creació: 10/03/25
 * @Data de l'última modificació: 11/06/25
 *************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EMAILS 100  // Número máximo de emails que puede procesar el programa

/***************************************************
* Estructura que representa un email con sus características
***************************************************/
typedef struct {
  int id;              // Identificador único del email
  char email[100];     // Dirección de correo electrónico
  char category;       // Categoría: I (importante), P (personal), U (desconocido)
  int links;           // Número de enlaces en el email
  char isSpam;         // Indicador de spam: 'y' (sí), 'n' (no)
  char dataType;       // Tipo de dato: '#' (entrenamiento), '*' (prueba)
} Email;

/***************************************************
* Estructura para almacenar los datos de predicción
***************************************************/
typedef struct {
  int data_id;    // ID del email al que corresponde la predicción
  char is_spam;   // Predicción: 'y' (es spam), 'n' (no es spam)
  float time;     // Tiempo que tomó realizar la predicción (en segundos)
} Prediction;

/***************************************************
* @Finalitat: Muestra el menú principal de opciones del programa
* @Paràmetres: No tiene parámetros
* @Return: No retorna ningún valor
***************************************************/
void printMenu() {
  printf("\n1) Load training data\t3) Calculate model metrics\t5) Close\n");
  printf("2) Filter by category\t4) Manual prediction\n");
  printf("Enter option: ");
}

/***************************************************
* @Finalitat: Valida que un valor numérico esté por encima del valor mínimo requerido
* @Paràmetres: 
*    - info: Mensaje que se muestra al usuario para solicitar el dato
*    - minValue: Valor mínimo permitido (inclusive)
* @Return: El valor numérico validado
***************************************************/
float validateNumericInput(char info[], float minValue) {
  char input[100];
  int valid = 0;
  float value = 0.0;
  
  while (valid == 0) {
    printf("%s", info);
    fgets(input, 100, stdin);
    value = atof(input);

    if (value < minValue) {
      printf("\nERROR: Value cannot be negative.\n");
      printf("\n");
    } else {
      valid = 1;
    }
  }
  
  return value;
}

/***************************************************
* @Finalitat: Evalúa una predicción contra el valor real y muestra el resultado
* @Paràmetres:
*    - actualIsSpam: Valor real: 'y' (es spam), 'n' (no es spam)
*    - predictedIsSpam: Valor predicho: 'y' (es spam), 'n' (no es spam)
* @Return: No retorna ningún valor
***************************************************/
void evaluatePrediction(char actualIsSpam, char predictedIsSpam) {
  if (actualIsSpam == 'y') {
    if (predictedIsSpam == 'y' || predictedIsSpam == 'Y') {
      printf("\nPrediction is correct (TP).\n");  // Verdadero Positivo
    } else {
      printf("\nIncorrect prediction (FN).\n");   // Falso Negativo
    }
  } else {
    if (predictedIsSpam == 'y' || predictedIsSpam == 'Y') {
      printf("\nIncorrect prediction (FP).\n");   // Falso Positivo
    } else {
      printf("\nPrediction is correct (TN).\n");  // Verdadero Negativo
    }
  }
}

/***************************************************
* @Finalitat: Carga los datos de emails desde un archivo
* @Paràmetres:
*    - filename: Nombre del archivo de datos
*    - emails: Array donde se cargarán los emails
*    - emailCount: Variable donde se guardará el número de emails cargados
* @Return: 1 si la carga fue exitosa, 0 en caso contrario
***************************************************/
int loadEmails(char filename[100], Email emails[MAX_EMAILS], int emailCount[1]) {
    int count = 0;
    int flag = 1;
    int valid = 0; 
    char aux[250];

    FILE *file;
    file = fopen(filename, "r");  // Abrir archivo en modo lectura

    if (file == NULL) {
        printf("ERROR: Could not open %s.\n", filename);
    } else {
        valid = 1;
        fgets(aux, 250, file);  // Leer la primera línea (encabezado)

        // Leer emails hasta alcanzar el máximo o fin de archivo
        while (count < MAX_EMAILS && flag == 1) {
            if (fscanf(file, "%d", &emails[count].id) != 1) {
                flag = 0;  // No se pudo leer más IDs, fin del archivo
            } else {
                // Leer los campos del email
                fscanf(file, "%s", emails[count].email);
                fscanf(file, " Category: %c", &emails[count].category);
                fscanf(file, " # links: %d", &emails[count].links);
                fscanf(file, " Is spam? %c", &emails[count].isSpam);
                fscanf(file, " [%c]", &emails[count].dataType);
                count = count + 1;
            }
        }

        fclose(file);
        emailCount[0] = count;  // Guardar el número total de emails leídos
    }

    return valid; 
}

/***************************************************
* @Finalitat: Carga y muestra los datos de entrenamiento (emails marcados con '#')
* @Paràmetres:
*    - emails: Array con los emails cargados
*    - totalEmails: Número total de emails en el array
* @Return: No retorna ningún valor
***************************************************/
void loadTrainingData(Email emails[MAX_EMAILS], int totalEmails) {
  int trainingCount = 0;
  int i = 0;
  char prefix;

  // Contar elementos de entrenamiento
  while (i < totalEmails) {
    if (emails[i].dataType == '#') {
      trainingCount = trainingCount + 1;
    }
    i = i + 1;
  }

  printf("\nTraining data elements: %d.\n", trainingCount);

  // Mostrar datos de entrenamiento
  i = 0;
  while (i < totalEmails) {
    if (emails[i].dataType == '#') {
      prefix = ' ';
      if (emails[i].isSpam == 'y') {
        prefix = '!';  // Marca los emails de spam con '!'
      }
      printf("\t%c [%c] %d %s: %d links\n", prefix, emails[i].category,
             emails[i].id, emails[i].email, emails[i].links);
    }
    i = i + 1;
  }
}

/***************************************************
* @Finalitat: Filtra y muestra los emails por categoría seleccionada por el usuario
* @Paràmetres:
*    - emails: Array con los emails cargados
*    - totalEmails: Número total de emails en el array
* @Return: No retorna ningún valor
***************************************************/
void filterByCategory(Email emails[MAX_EMAILS], int totalEmails) {
  char input[100];
  char category;
  int i = 0;
  int found = 0;
  int validCategory = 0; 
  char type[10];

  // Solicitar categoría hasta obtener una válida
  while (validCategory == 0) { 
    printf("\nCurrent categories:\n\t[I]mportant\n\t[P]ersonal\n\t[U]nknown\n");
    printf("Filter by: ");
    printf("\n");
    fgets(input, 100 , stdin);
    category = input[0];

    // Convertir a mayúscula si es necesario
    if (category >= 'a' && category <= 'z') {
      category = category - ('a' - 'A'); 
    }

    // Validar la categoría
    if (category == 'I' || category == 'P' || category == 'U') {
      validCategory = 1; 
    } else {
      printf("ERROR: Invalid category.\n");
    }
  }

  // Buscar y mostrar emails de la categoría seleccionada
  while (i < totalEmails) {
    if (emails[i].category == category) {
      found = 1;

      // Determinar si es dato de entrenamiento o prueba
      if (emails[i].dataType == '#') {
        strcpy(type, "TRAIN");
      } else {
        strcpy(type, "TEST");
      }

      // Mostrar info del email (! indica spam)
      if (emails[i].isSpam == 'y') {
        printf("\t! (%d) %s - %d links [%s]\n", emails[i].id, emails[i].email,
               emails[i].links, type);
      } else {
        printf("\t  (%d) %s - %d links [%s]\n", emails[i].id, emails[i].email,
               emails[i].links, type);
      }
    }
    i = i + 1;
  }

  // Informar si no se encontraron emails en esa categoría
  if (found == 0) {
    printf("No data could be found.\n");
  }
}

/***************************************************
* @Finalitat: Busca un email por su ID en el array de emails
* @Paràmetres:
*    - emailID: ID del email a buscar
*    - emails: Array con los emails cargados
*    - totalEmails: Número total de emails en el array
* @Return: Índice del email en el array si se encuentra, -1 en caso contrario
***************************************************/
int findEmailById(int emailID, Email emails[MAX_EMAILS], int totalEmails) {
  int i = 0;
  int found = 0;
  int emailIndex = -1;
  
  while (i < totalEmails && found == 0) {
    if (emails[i].id == emailID) {
      found = 1;
      emailIndex = i;
    }
    i = i + 1;
  }
  
  return emailIndex;
}

/***************************************************
* @Finalitat: Calcula métricas del modelo a partir de predicciones en un archivo binario
* @Paràmetres:
*    - emails: Array con los emails cargados
*    - totalEmails: Número total de emails en el array
* @Return: No retorna ningún valor
***************************************************/
void calculateModelMetrics(Email emails[MAX_EMAILS], int totalEmails) {
  char filename[250];
  FILE *file;
  Prediction prediction;
  int tp, tn, fp, fn;  // Verdaderos/Falsos Positivos/Negativos
  float totalTime = 0.00, accuracy = 0.00, precision = 0.00, recall = 0.00, avgTime = 0.00;
  int i, validPredictions;

  // Solicitar nombre de archivo de predicciones
  printf("\nWhere are the prediction results? ");
  fgets(filename, 250, stdin);
  filename[strcspn(filename, "\n")] = 0;  // Eliminar salto de línea

  // Abrir archivo en modo binario
  file = fopen(filename, "rb");
  if (file == NULL) {
    printf("\nERROR: Could not open file.\n");
  } else {
    tp = 0;
    tn = 0;
    fp = 0;
    fn = 0;
    totalTime = 0.0;
    validPredictions = 0;

    // Leer predicciones y calcular métricas
    while (fread(&prediction, sizeof(Prediction), 1, file) == 1) {
      // Buscar el email correspondiente por ID usando la nueva función
      i = findEmailById(prediction.data_id, emails, totalEmails);

      if (i == -1) {
        printf("[WARNING] Email %d is not in data file.\n", prediction.data_id);
      } else {
        // Procesar predicción válida
        validPredictions = validPredictions + 1;
        totalTime = totalTime + prediction.time;

        // Actualizar contadores de TP, TN, FP, FN
        if (prediction.is_spam == 'y') {
          if (emails[i].isSpam == 'y') {
            tp = tp + 1;  // Verdadero Positivo
          } else {
            fp = fp + 1;  // Falso Positivo
          }
        } else {
          if (emails[i].isSpam == 'y') {
            fn = fn + 1;  // Falso Negativo
          } else {
            tn = tn + 1;  // Verdadero Negativo
          }
        }
      }
    }

    fclose(file);

   // Calcular métricas si hay predicciones válidas
   if (validPredictions > 0) {
      // Exactitud: porcentaje de predicciones correctas
      accuracy = ((tp + tn) * 100.0) / (tp + tn + fp + fn);
      
      // Precisión: porcentaje de positivos verdaderos entre los predichos como positivos
      if ((tp + fp) > 0) {
        precision = (tp * 100.0) / (tp + fp);
      } else {
        precision = 0.0;
      }

      // Recall: porcentaje de positivos verdaderos identificados correctamente
      if ((tp + fn) > 0) {
        recall = (tp * 100.0) / (tp + fn);
      } else {
        recall = 0.0;
      }

      avgTime = totalTime / validPredictions;
    } else {
      accuracy = 0.0;
      precision = 0.0;
      recall = 0.0;
      avgTime = 0.0;
    }

    // Mostrar métricas calculadas
    printf("\nAccuracy: %.2f percent\n", accuracy);
    printf("Precision: %.2f percent\n", precision);
    printf("Recall: %.2f percent\n", recall);
    printf("Average prediction time: %.2fs\n", avgTime);
  }
}

/***************************************************
* @Finalitat: Realiza una predicción manual de spam y evalúa su resultado
* @Paràmetres:
*    - emails: Array con los emails cargados
*    - totalEmails: Número total de emails en el array
* @Return: No retorna ningún valor
***************************************************/
void manualPrediction(Email emails[MAX_EMAILS], int totalEmails) {
  int emailID, emailIndex, valid;
  float predictionTime;
  char isSpam;
  char input[100];

  emailIndex = -1;
  valid = 0;

  // Solicitar ID del email hasta obtener uno válido
  while (emailIndex == -1) {
    printf("\nEnter the ID of the email: ");
    fgets(input, 100, stdin);
    emailID = atoi(input);

    if (emailID < 0) {
      printf("\nERROR: ID cannot be negative.\n");
    } else {
      emailIndex = findEmailById(emailID, emails, totalEmails);
      if (emailIndex == -1) {
        printf("\nERROR: ID not found.\n");
      }
    }
  }

  // Solicitar tiempo de predicción y validarlo
  valid = 0;
  while (valid == 0) {
    printf("Enter the prediction time (in seconds): ");
    fgets(input, 100, stdin);
    predictionTime = atof(input);

    if (predictionTime < 0) {
      printf("\nERROR: Time cannot be negative.\n");
      printf("\n");
    } else {
      valid = 1;
    }
  }

  // Solicitar predicción (spam o no spam)
  valid = 0;
  while (valid == 0) {
    printf("Did the model predict it as spam (y/n)? ");
    fgets(input, 100, stdin);
    isSpam = input[0];

    if (isSpam == 'y' || isSpam == 'Y' || isSpam == 'n' || isSpam == 'N') {
      valid = 1;
    } else {
      printf("\nERROR: Invalid choice.\n");
      printf("\n");
    }
  }

  // Evaluar y mostrar resultado de la predicción
  if (emails[emailIndex].isSpam == 'y') {
    if (isSpam == 'y' || isSpam == 'Y') {
      printf("\nPrediction is correct (TP).\n");
    } else {
      printf("\nIncorrect prediction (FN).\n");
    }
  } else {
    if (isSpam == 'y' || isSpam == 'Y') {
      printf("\nIncorrect prediction (FP).\n");
    } else {
      printf("\nPrediction is correct (TN).\n");
    }
  }
}

/***************************************************
* @Finalitat: Función principal del programa que gestiona el detector de spam
* @Paràmetres: No tiene parámetros
* @Return: 0 si el programa finaliza correctamente
***************************************************/
int main() {
  Email emails[MAX_EMAILS];
  int totalEmails = 0;
  char filename[100];
  char input[100];
  int option;
  int valid;

  // Solicitar archivo de datos
  printf("Data filename: ");
  fgets(filename, 100, stdin);
  filename[strlen(filename) - 1] = '\0';  // Eliminar salto de línea

  // Cargar emails desde archivo
  valid = loadEmails(filename, emails, &totalEmails);

    if (valid == 1) {

  printf("\nWelcome to Spam Detector!\n");

  // Ciclo principal del programa
  do {
    printMenu();
    fgets(input, 100, stdin);
    option = atoi(input);

    // Procesar opción seleccionada
    switch (option) {
    case 1:
      loadTrainingData(emails, totalEmails);
      break;
    case 2:
      filterByCategory(emails, totalEmails);
      break;
    case 3:
      calculateModelMetrics(emails, totalEmails);
      break;
    case 4:
      manualPrediction(emails, totalEmails);
      break;
    case 5:
      printf("\nClosing Spam Detector...\n");
      break;
    default:
      printf("\nERROR: Invalid option.\n");
      break;
      }
      
    } while (option != 5);
  }
  return 0;
}
