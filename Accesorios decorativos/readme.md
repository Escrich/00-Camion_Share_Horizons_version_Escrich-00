# Accesorios añadidos o modificados

### Pala excavadora

Esta pala se basa totalmente en el diseño original, tan solo se le han añadido los dientes, y se han movido hacia el centro los dos grupos de taladros de sujección, disminuyendo la distancia entre taladros en ocho milimetros, 
esta diferencia de ocho milimetros de envergadura, proviene de un exceso en la medida original, y del hecho de haber rebajado el brazo del servo, por el lado contrario al piñon, hasta dejarlo plano.

En ambos camiones está impresa en Super PLA plus.


### Escalera de bomberos

La escalera es para aprovechar el color rojo de uno de mis dos camiones, para darle un aspecto similar al de un camión de bomberos, lejos de pretender una recreación realista, es mas bien algo conceptual, o casi un elemento de comic,
junto a la escalera están los soportes que la sujetan y acompañan, en los soportes de la parte superior, se montan insertos metálicos de M3 usando calor, estos insertos, se montan desde lo que sería la parte superior, 
por contra el tornillo que los sujeta, viene desde dentro de la carcasa del camion, haciendo, según mi practica habitual, que los insertos queden de forma antagonista al tornillo,
y que no sea posible arrancarlos, por mas que se apriete el mencionado tornillo, sino que la tendencia, sea a clavarse cada vez mas en la pieza, evitando que se pueda soltar, 
aquí no es necesaria, quizás, tanta sujección, pero es mas un modo de trabajar que aplico siempre que es posible, a todos los insertos metálicos que monto en mis diseños.

La escalera y sus piezas de sujección, han sido impresas en PETG.


### Antena de telecomunicaciones

La antena es solo un adorno que dibujé hace años, como parte de los diseños para un barco de RC, y al igual que en el caso de la escalera es solo un concepto, poco realista, pero entendible, de lo que es una antena
lleva en su interior un inserto metalico de M3, montado a la manera tradicional, para no dejar un taladro visible desde la parte superior, y una pieza decorativa circular como base, 
se atornilla tambien desde dentro del camión, usando la carcasa modificada, que denomino carcasa multiusos, y que tiene ya preparados los taladros para la escalera, la antena o cualquier otro uso que la imaginación te sugiera.

La antena está impresa en Super PLA plus, y la pieza decorativa de la base en PLA con fibra de carbono, (PLA CF).


### Rotativo luminoso

Aunque no hay ningún tipo de rotación mecanica, se trata de un rotativo como el de una ambulancia, o coche de bomberos, donde la sensación de rotación se consigue con el movimiento aparente, que se logra encendiendo y apagando leds del tipo Neopixel,
para ello en el programa se ha añadido un nuevo canal Neopixel, usando D5 como salida de control, y se ha implementado con cuatro leds independientes, no en una tira led, cableados uno a continuación del otro, 
el "rotativo" hace tres funciones diferentes, las dos primeras muy similares, en caso de no haber conexión con el mando, mediante una rutina simple se encienden en rojo, dos a dos, los dos leds delanteros, o los dos traseros, mientras los otros dos permanecen apagados,
una vez que se produce la conexión entre el camión y el mando a distancia, la rutina es la misma, pero esta vez con la luz en verde.
La diferencia real viene al poner los motores en marcha, aquí la estructura del programa es diferente a los dos casos anteriores, y se pueden programar facilmente, multiples pasos con multiples combinaciones de luces y colores, simplemente añadiendo subestructuras case / break,
una vez estas se agoten, el programa simplemente saldrá y si las condiciones se mantienen, volverá a ejecutar toda la secuencia de colores desde el principio.

Tras muchas pruebas, poner luces en plan "coche fantastico", y hacer muchas combinaciones, al final solo hay dos combinaciones de rojo y azul, que se alternan entre si, con lo que el programa de luces en movimiento solo tiene dos pasos, a pesar de mantener la estructura para poder crear multiples pasos y cambios de luz, lo que la convierte en una estructura de programa, facilmente exportable, para poder ser utilizada en otros proyectos.

Todo el conjunto va sujeto con dos tornillos de M2'5, que vienen desde la pieza de soporte de los servos, previamente modificada, con los taladros de sujección y la salida de cables hacia el interior del camión, los insertos de M2'5, se montan en las dos piezas transparentes/translucidas, desde la parte de arriba, para ser atornilladas desde abajo, siguiendo la misma filosofia explicada previamente para los insertos.

Las luces propiamente dichas, en ambos camiones, están impresas en Transparent PETG, y los soportes, en el caso del camión de bomberos es PETG, pero en el caso del camióncito con antena, se trata de nuevo de PLA CF.


### Tapa leds gruesa

Es la misma tapa original, pero aumentada en grosor en un 50%

Está impresa en un solo color, usando transparent PETG

<sub> 
20250509 Jose M. Escrich 
</sub>
