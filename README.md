# Proyecto_Final_ISO
link del video: https://www.youtube.com/shorts/iT2Wpvigz9c

Caracteristicas del sistema operativo
-------------------------------------
Es un sistema operativo  del tipo expropiativo.

APIS que brinda el SO son :tasks,delay,queue,semaforos binarios,control de interrupciones.

API referencias
-------------
Tareas
-----
Para crear un tarea se utiliza la funcion os_control_add_task(task_function fn_task, void*parameter, uint8_t priority, t_os_task* task, t_node* node) que resibe como parametros la funcion de la tarea,un puntero a un argumento que se le desee pasar,la prioridad,una estructura de control para la tarea de tipo t_os_task, una estructura de tipo t_node.

El sistem operativo tiene implementa una tarea idle se crea automaticamente,esta tarea utiliza el SO cuando todas la demas tareas esta bloqueas, se podria usar para colocar al sismtema en modo ahorro de energia.
Se crearon los hooks del sistema que se podrian utilizar para diferentes funciones :

tick_hook(): Se llama cada tick del sistema

idle_hook(): Funcion del tarea IDLE

error_hook(): Utilizada para manejar errores del sistema 

PRIORIDADES
-------------
Se implemetaron prioridades para las tareas que van desde 0 a 4, donde 4 es la de mayor prioridad.
Al crear las tareas las proridades se deben asignar colocando TASK_IDLE_PRIORITY+el valor de la prioridad, por que se le asigno la prioridad de 0 a la tarea IDLE.

DELAY
------------
Se utiliza para retrasar un tarea un determinado numero de ticks del sistema
para utilizar los delays se llama la funcion delay_task(uint32_t ticks) que recibe como parametro el numero de ticks a esperar.
La tarea que lleme a la funcion de delay cambio su estado a BLOKED 

SEMAFOROS BINARIOS
------------------
Se utilizan para sincronizar dos tares.
Para crear un semaforo se utiliza la funcion semaphore_init(t_semaphore* semaphore) que resibe como parametro u puntero al un estrutura de tipo t_semaphore.
La forma de uso de los semaforos es la siguiente:

Si una tarea quiere tomar el semaforo utiliza la funcion semaphore_take(t_semaphore* semaphore) si el semaforo ya esta tomado la tarea se bloquea.
Si una tarea quiere dar un semaforo utiliza la funcion semaphore_give(t_semaphore *semaphore)

Colas
---------------
Las colas son elementos de sincronizacion y de comunicacion entre tareas.
Para crear un cola se utiliza la funcion init_queue(t_os_queue* queue, uint8_t* head, uint8_t amount_items, uint8_t size_item) que recibe como parametros uns structura de control de  tipo t_os_queue, el puntero al buffer de la cola, catidad de elementos que almacenara la cola, el tamano en bytes de cada elemento de la cola.

La forma de uso de las colas es la siguiente:

Para enviar un dato por la cola se utiliza la funcion queue_send(t_os_queue* queue, void* data)que recibe como parametros la cola donde enviar y el dato para enviar, si la cola esta llena la tarea que llamo la funcion se bloquea.

Para recibir un dato se utiliza la funcion queue_receive(t_os_queue* queue, void* data)
que recibe como parametros la cola de donde se va recivir y donde se va almacenar el dato,
si la cola esta vacia la tarea se bloquea esta que haya un dato. 

Manejo de interrupciones
-----------------------
Si se desea utilizar un interrupcion primero se debe intalar en el sistema operativo dicha interrupcion porque ahora el SO es el que maneja todos lo handlers de interrupciones.

Se utiliza la funcion os_install_irq(IRQn_Type irq,void* isr) que recibe como parametros la irq deseada y la funcion del callback para que se ejecute cuando handler se ejecute.