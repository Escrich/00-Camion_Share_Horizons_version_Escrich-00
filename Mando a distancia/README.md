# Mando a distancia Escrich

<br>
<p align="left">
<img src="https://github.com/Escrich/00-Camion_Share_Horizons_version_Escrich-00/blob/master/Mando%20a%20distancia/mando%20a%20distancia.jpg" alt='Mando a distancia versión Escrich' width='50%'>
</p>
<br>

Aquí podeis encontrar toda la información relativa al mando a distancia, desde el programa, lleno de comentarios con los cambios efectuados, y una explicacion de dichos cambios, en la mayoría de los casos, 
hasta las instrucciones de montaje y puesta en marcha, paso a paso.

Tambien están, por supuesto, los ficheros .step de todas las piezas que componen el mando a distancia, así como los ficheros .obj o .stl, pero lo que mas trabajo me ha costado, 
es la guía de construcción, donde se explica paso a paso como construirlo, como y cuando programarlo, y como cablearlo, ajustarlo y hacerlo funcionar.

La forma de programarlo, que está descrita paso a paso, es la descrita en el documento original de **Share Horizons**, y es exactamente la misma que para el **DOIT ESP32 DEV KIT V1**, pero eligiendo una placa, o la otra, ya que el microcontrolador que lleva dentro, 
un **Wemos D1 Mini ESP32**, es **100% compatible**, con el **ESP32** del diseño original.


Programarlo, es tan simple como conectar un cable USB entre tu ordenador y el **Wemos D1 Mini**, cargar el programa que encontrarás aquí, en el entorno **Arduino**, si es que no lo habías hecho antes, 
elegir la placa, (**Wemos D1 Mini ESP32**) y el **puerto correctos**, 
asegurarte de que has puesto la **mac address** de la placa de tu camión correctamente, y pulsar en el entorno **Arduino** el **icono de compilar y cargar**, 
si lo has hecho todo bien, el sistema saldrá andando a la primera, que por otra parte, es lo mas normal.



> [!IMPORTANT]
> 20250827 Actualización del software del mando a distancia, como consecuencia de las actualizaciones de las librerías de Arduino.
>


> [!WARNING]
> Al construir el mando a distancia, no olvides, antes de aplicar tension al Wemos D1 MINI, haber ajustado externamente la fuente de alimentación, para una tensión de salida de entre **5'0 Voltios, y 5'1 Voltios**.
> si no lo haces así, puedes destruir el microcontrolador, al aplicarle una tensión mas alta de la que puede aguantar.

<br>

> [!CAUTION]
> Recuerda alimentar al Joystick con tan **solo 3'3 Voltios**, desde los pines de la placa del Wemos, o ESP32,
> Es un problema muy común, alimentarla a 5 voltios, tal como pone en su serigrafía, haciendolo así, el camión se pondrá en marcha en cuanto conecte con el mando, y notarás una sensación de total falta de control.



<br>
 
www.github.com/escrich
  
https://t.me/escrich
  
https://www.tiktok.com/@josemescrich

<sub> 
20250509 Jose M. Escrich 
</sub>
