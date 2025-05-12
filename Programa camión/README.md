# Programa del camión Share Horizons versión Escrich

Aquí podeis encontrar información relativa al programa del camión, una vez reformado por mi, que insisto, no es mejor ni peor, tan solo diferente al original, y que conserva una buena parte de lo que ya había.

- La forma de programarlo, que está descrita paso a paso, es exactamente la misma para el DOIT ESP32 DEV KIT V1, que para el Wemos D1 Mini ESP32, que es el micro que lleva mi versión del mando a distancia,
  una vez en el entorno Arduino, solo hay que elegir la placa y puerto correctos, hay una buena descrpcion de como hacerlo en las instrucciones del mando a distancia. 

- Por cierto el Wemos D1 Mini ESP32, es 100% compatible, con el ESP32 del diseño original.


- Programarlo es tan simple como conectar un cable USB entre tu ordenador y el ESP32, cargar el programa que encontrarás aquí, en el entorno Arduino, si es que no lo habías hecho antes, 
elegir la placa, (DOIT ESP32 DEV KIT V1) y el puerto correctos, 
y pulsar en el entorno Arduino el icono de compilar y cargar, 
si lo has hecho todo bien, el sistema saldrá andando a la primera, que por otra parte, es lo mas normal.

 - En esta versión se ha cambiado, comprobado y corregido, la nomenclatura de todas las variables de dirección y control de motor, para que coincidan con la dirección en la que se mueve el camión.

 - He recalculado los angulos de la dirección, haciendolos simetricos, tanto a izquierda y derecha, como adelante y atrás.

 - Tambien se han añadido rutinas para el cambio de color de los leds en función de la dirección, que ahora al moverse, hay dos leds de la tira, que se vuelven azules, y que indican en que dirección vamos,
   por eso es importante el orden y la forma de montaje, a la hora de poner la tira led, por otra parte, aunque está hecho para 8 leds, es muy sencillo editarlo para que funcione con los 12 leds del diseño original.

 - Añadidos nuevos leds Neopixel en el canal 5, con los que se hace el efecto de luces de policia, son cuatro leds inteligentes Neopixel, 
  conectados uno a continuación de otro, y programados para hacer ese efecto, con una sección de programa abierta a hacer las combinaciones que quieras, como ya digo en otro sitio, es una versión, muy exportable a cualquier otro proyecto.

 - También puedes desde el mismo programa, sin necesidad de un programa auxiliar, mostrar la dirección Mac Address de tu camión, imprescindible para incluirla en el programa del mando a distancia

 - Otra cosa que tambien he añadido, tanto en el mando como en el camión, es una cosa mas bien estética, el led azul de la placa, ahora,
   se enciende o apaga a cada ciclo de programa, para poder comprobar de manera visual, que el programa está funcionando y no se ha quedado colgado.





> [!TIP]
> Aunque aquí te deje alguna explicación sobre el software del camión, la mayor parte de las reformas, por no decír todas,
> están comentadas y documentadas con texto y comentarios, dentro del propio programa, (no me gusta llamarle "sketch")
>

https://www.github.com/escrich


<sub> 
20250509 Jose M. Escrich 
</sub>
