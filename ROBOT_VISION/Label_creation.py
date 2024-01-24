import pandas as pd
import os
import csv

# Lee el archivo Excel (asegúrate de tener la librería pandas instalada: pip install pandas)
archivo_csv = os.path.normpath('C:/Users/andre/Desktop/Vision_Robot/Code/OIDv4_ToolKit/OID/csv_folder/train-annotations-bbox.csv')

# Define el diccionario de mapeo para la columna C
mapeo_columna_C = {
    '/m/02pv19': '0',
    '/m/01pns0': '1',
    '/m/01g317': '2',
    '/m/0jbk': '3',
    '/m/08dz3q': '4',
    '/m/0dzct': '5'
}


with open(archivo_csv, 'r') as f:
    line = f.readline()
    while len(line) != 0:
        id, _, class_name, _, x1, x2, y1, y2, _, _, _, _, _ = line.split(',')[:13]
        # Obtén el nombre del archivo (columna A)
        nombre_archivo = id

        # Obtén el valor de la columna C
        valor_columna_C = class_name

        # Mapea el valor de la columna C según las reglas especificadas
        ruta_img = f'C:/Users/andre/Desktop/Vision_Robot/Code/Dataset/images/train/{nombre_archivo}.jpg'
        if valor_columna_C in mapeo_columna_C and os.path.exists(ruta_img):
            contenido_txt = mapeo_columna_C[valor_columna_C]
            # Agrega los valores de las columnas E, F, G, H si existen
            contenido_txt += f" {x1} {x2} {y1} {y2}"

            # Crea el archivo de texto con el nombre de la columna A y escribe el contenido
            ruta_txt = f'{nombre_archivo}.txt'
            if os.path.exists(ruta_txt):
                # Si existe, agrega la información al archivo con un salto de línea
                with open(ruta_txt, 'a') as archivo_txt:
                    archivo_txt.write('\n' + contenido_txt)
            else:
                # Si no existe, crea el archivo de texto y escribe el contenido
                with open(ruta_txt, 'w') as archivo_txt:
                    archivo_txt.write(contenido_txt)


        line = f.readline()
