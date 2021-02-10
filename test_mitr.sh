#! /bin/bash
echo ">>>>>> TEST MITR <<<<<<"
sleep 1
echo "Realizando pruebas..."
#Prueba1: Sustituir una caracter , dos ficheros existentes y file2 vacío
./mitr a A texto.txt vacio.txt
cat texto.txt | tr 'a' 'A' > trim.txt
if  [[ $(diff vacio.txt trim.txt) -ne 0 ]];  then 
	echo "Error en prueba1"
fi

#Prueba2: Sustituir dos caracteres , dos ficheros existentes 
./mitr ES es texto.txt novacio.txt
cat texto.txt | tr 'ES' 'es' > trim.txt
if  [[ $(diff novacio.txt trim.txt) -ne 0 ]];  then 
	echo "Error en prueba2"
fi

#Prueba3: Sustituir cadenas, file2 no existe
./mitr llover LLOVER  texto.txt noexiste.txt >/dev/null
if  [[ $(diff noexiste.txt trim.txt) -ne 0 ]];  then 
	echo "Error en prueba3"
fi

#Prueba4: Sustituir cadenas, mismo fichero, mismo contenido de texto.txt
./mitr LLOvER lloTer texto2.txt texto2.txt
if  [[ $(diff texto2.txt trim.txt) -ne 0 ]];  then 
	echo "Error en prueba4"
fi

#Prueba5: file1 no existe, salta error
./mitr ES es noexiste1.txt novacio.txt 2> /dev/null
if [[ ! $(echo "$?") -ne 0 ]]  ; then 
	echo "Error en prueba5"
fi
#Prueba6: Parámetros distinto de 4, salta error
./mitr ES es texto.txt 2> /dev/null
if [[ ! $(echo "$?") -ne 0 ]]  ; then 
	echo "Error en prueba6"
fi

#Prueba7: Cadenas distinto tamaño, salta error
./mitr Wikipedia W texto.txt texto.txt 2> /dev/null
if [[ ! $(echo "$?") -ne 0 ]]  ; then 
	echo "Error en prueba7"
fi

#Prueba8: Ningún parámetro, salta error
./mitr 2> /dev/null 
if [[ ! $(echo "$?") -ne 0 ]]  ; then 
	echo "Error en prueba8"
fi

#Prueba9: sin permiso en file2, salta error
./mitr texto.txt nowrite.txt 2> /dev/null 
if [[ ! $(echo "$?") -ne 0 ]]  ; then 
	echo "Error en prueba9"
fi

#Prueba10: carpeta y documento, salta error 
./mitr a A carpeta novacio.txt 2> /dev/null 
if [[ ! $(echo "$?") -ne 0 ]]  ; then 
	echo "Error en prueba10"
fi

sleep 1
echo "Fin pruebas..."
exit 0