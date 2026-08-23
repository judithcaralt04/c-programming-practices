/***********************************************
*
* @Propòsit: Realizar la práctica 1.1
* @Autor/s: Judith Caralt Rico
* @Data de creació: 12/11/24
* @Data de l’última modificació: 14/11/24
*
************************************************/

#include <stdio.h> 

#define LOW_WEIGHT 18.5 
#define NORMAL_WEIGHT 24.9 

/***********************************************
*
* @Finalitat: Función principal que gestiona el sistema de control del peso de 
* los niños.
* @Paràmetres: No tiene ningún parametro de entrada.
* @Retorn: 0 indicando una salida normal del programa.
*
************************************************/

int main() {
  int id, valid_age = 0;                                                        // Variables de identificador y validación de edad
  char option;                                                                  // Variable de opción de menú
  float height, weight, bmi;                                                    // Variables de altura, peso y BMI
  int age;                                                                      // Variable de edad
  float weight_year, weight_weeks, estimated_weight;                            // Variables para cálculos de ganancia de peso
  int low_weight = 0, normal_weight = 0, overweight = 0;                        // Contadores para categorías de BMI
  int child_count = 0;                                                          // Contador de niños
  int consulting_weeks;                                                         // Variable para semanas de consulta
  int bmi_calculated = 0, children_bmi = 0;                                     // Contadores para niños con BMI calculado

  // Validación de identificador de doctor
  printf("Doctor number: "); 
  scanf("%d", &id); 
  if (id < 100000 || id > 999999) {                                             // Verifica si el número de doctor tiene 6 dígitos
    printf("Invalid doctor number! (must have 6 digits)\n");                    // Mensaje de error si no es válido
  } 
  else {
    printf("Welcome to Healthy Child Program! (Dr. id: %d)\n", id);             // Mensaje de bienvenida si el número es válido

    do {
      // Menú principal
      printf("\n");                                                             // Salto de línea
      printf(
          "a. Add child data | b. Calculate BMI | c. Statistics | d. Exit\n"); 
      printf("Select option: "); 
      scanf(" %c", &option); 

      switch (option) { 
      case 'A': 
      case 'a': 
        // Opción 1: Añadir datos de un niño
        do {
          printf("Child's age: ");                                             
          scanf("%d", &age);
          if (age < 1 || age > 12) {                                            // Verifica si la edad está en el rango permitido
            printf("The patient is not a child. Enter the age again!\n");       // Mensaje de error si no es válido
          } 
          else {
            valid_age = 1;                                                      // Marca que la edad es válida
          }
        } while (!valid_age);                                                   // Bucle que se repite hasta que la edad sea válida

        do {
          printf("Enter height (cm): "); 
          scanf("%f", &height); 
          if (height <= 0) {                                                    // Verifica si la altura es positiva
            printf("The height must be a positive value!\n");                   // Mensaje de error si no es válido
          }
        } while (height <= 0);                                                  // Bucle que se repite hasta que la altura sea válida

        do {
          printf("Enter weight (lb): "); 
          scanf("%f", &weight); 
          if (weight <= 0) {                                                    // Verifica si el peso es positivo
            printf("The weight must be a positive value!\n");                   // Mensaje de error si no es válido
          }
        } while (weight <= 0);                                                  // Bucle que se repite hasta que el peso sea válido

        child_count++;                                                          // Incrementa el contador de niños registrados en el sistema
        break; 

      case 'B': 
      case 'b': 
        // Opción 2: Calcular recomendaciones de BMI
        if (child_count == 0) {                                                 // Verifica si hay datos de niños registrados en el contador
          printf("BMI cannot be calculated until data is "
                 "inserted.\n");                                                // Mensaje en el caso de que no haya datos
        } 
        else {
          // Calcular el BMI del niño ingresado
          bmi = (weight / 2.2) / ((height / 100.0) * (height / 100.0));         // Cálculo del BMI
          printf("BMI: %.1f\n", bmi); 
          children_bmi++;                                                       // Incrementa el contador de niños con BMI calculado
          // Contar las estadísticas según el BMI
          if (bmi < LOW_WEIGHT) {                                               // Verifica si el niño tiene peso bajo
            printf("This child must follow our nutrition plan!\n");             // Mensaje si el niño necesita un plan nutricional
            low_weight++;                                                       // Incrementa el contador de peso bajo
          } 
          else {
            if (bmi <= NORMAL_WEIGHT) {                                         // Verifica si el niño tiene peso normal
              printf("This child is at the right weight!\n");                   // Mensaje si el niño tiene peso normal
              normal_weight++;                                                  // Incrementa el contador de peso normal
            } 
            else {                                                            // Si el niño tiene sobrepeso
              printf(
                  "This child must perform a series of physical exercises!\n"); // Mensaje si el niño necesita ejercicios
              overweight++;                                                     // Incrementa el contador de sobrepeso
            }
          }

          if (bmi < LOW_WEIGHT) {                                               // Si el niño tiene peso bajo
            do {
              printf("Next consulting (weeks)? ");                              // Pide las semanas para la próxima consulta
              scanf("%d", &consulting_weeks); 
              if (consulting_weeks > 20) {                                      // Verifica si excede el límite de 20 semanas
                printf("ERROR! Must be less than 20 weeks.\n");                 // Mensaje de error si excede el límite
              } 
              else {
                if (age < 7) {                                                  // Determina la ganancia de peso anual según la edad
                  weight_year = 2.0;                                            // Ganancia de peso para niños menores de 7 años
                } 
                else {
                  weight_year = 2.4;                                            // Ganancia de peso para niños de 7 años o más
                }

                // Cálculo del peso estimado
                weight_weeks =
                    (weight_year / 52) * consulting_weeks;                      // Cálculo ganancia de peso en semanas
                estimated_weight =
                    weight + weight_weeks * 2.2;                                // Cálculo del peso en libras
                printf("In %d weeks the child should weigh %.2flb.\n",
                       consulting_weeks, estimated_weight); 
              }
            } while (consulting_weeks > 20);                                    // Bucle que se repite hasta que el número de semanas sea válido
          }
        }
        break; 

      case 'C':
      case 'c': 
        // Opción 3: Mostrar estadísticas
        if (child_count == 0 || bmi == 0.0) {                                   // Verifica si hay datos registrados y si hay bmi calculado para mostrar estadísticas
          printf("Statistics cannot be displayed until BMI has been "
                 "calculated.\n");                                              // Mensaje si no hay estadísticas disponibles
        } 
        else {
          printf("Child's statistics:\n"); 
          printf("Malnourished => %d (%.f%%)\n", low_weight,
                 (low_weight * 100.0) / children_bmi);                          // Muestra el porcentaje de niños con peso bajo
          printf("Normal => %d (%.f%%)\n", normal_weight,
                 (normal_weight * 100.0) / children_bmi);                       // Muestra el porcentaje de niños con peso normal
          printf("Overweight => %d (%.f%%)\n", overweight,
                 (overweight * 100.0) / children_bmi);                          // Muestra el porcentaje de niños con sobrepeso
          printf("TOTAL ==== %d\n", children_bmi);                              // Muestra el total de niños con BMI calculado
        }
        break; 

      case 'D': 
      case 'd': 
        // Opción 4: Salir
        printf("See you in the next appointment!!\n");                          // Mensaje de despedida
        break; 

      default:                                                                  // Caso por defecto si la opción introducida no es válida
        printf("Wrong option!\n");                                              // Mensaje de error para opción no válida
      }
    } while (option != 'D' && option != 'd');                                   // Bucle que repite el menú hasta que se elija la opción D (salir)
  }
  return 0;                                                                     
}

