#include "client.h"
#include "stdlib.h"
#include "stdio.h"
#include <readline/readline.h>


int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* clave;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	log_info(logger, "Soy un Log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip = config_get_string_value(config, "IP");
    puerto = config_get_string_value(config, "PUERTO");
    clave = config_get_string_value(config, "CLAVE");
	 
	// Loggeamos el valor de config
	log_info(logger, "IP: %s", ip);
    log_info(logger, "Puerto: %s", puerto);
    log_info(logger, "Clave: %s", clave);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje

	enviar_mensaje(clave, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* aux_logger = log_create("tp0.log", "cliente", true, LOG_LEVEL_INFO);
	if(aux_logger == NULL){
		perror("Falla al momento de iniciar logger.");
		exit(EXIT_FAILURE);
	}
	return aux_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	nuevo_config = config_create("/home/utnso/tp0/client/cliente.config");
    
    if (nuevo_config == NULL) {
        perror("Falla al iniciar el config");
        exit(EXIT_FAILURE);
	}	
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido = NULL;

	// La primera te la dejo de yapa
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while (leido  && strlen(leido) > 0) {
        log_info(logger, "%s", leido); // Loggear la entrada de la consola
        free(leido); // Liberar la memoria asignada por readline
        leido = readline("> "); // Leer la siguiente línea}
		}
	// ¡No te olvides de liberar las lineas antes de regresar!
	if (leido) {
        free(leido); // Liberar la memoria asignada por readline
    }


	

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido = NULL;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while (strcmp(leido, "") != 0) {
        agregar_a_paquete(paquete, leido, strlen(leido) +1);
        free(leido); // Liberar la memoria asignada por readline
        leido = readline("> "); // Leer la siguiente línea}
		}
	if (leido) {
        free(leido); // Liberar la memoria asignada por readline
    }
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	
	//Enviar paquete
	enviar_paquete(paquete, conexion);

	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	  log_destroy(logger);

	  // Destruir la configuración
 	   config_destroy(config);

	   liberar_conexion(conexion);
}