# Image Processing

Una herramienta simple de línea de comandos para rotar y escalar imágenes usando C++.

## Características

- Rotación de imágenes en ángulos específicos
- Escalado de imágenes a diferentes tamaños
- Soporte para múltiples formatos de imagen (PNG, JPEG, BMP)
- Interfaz de línea de comandos fácil de usar
- Manejo de memoria eficiente

## Requisitos

- Compilador de C++ (g++ recomendado)
- Make

## Instalación

1. Clona el repositorio:
    ```
    git clone https://github.com/JuanM0412/Image_Processing.git
    ```
2. Entra a la nueva capeta que se creó:
    ```
    cd Image_Processing
    ```

## Compilación
1. Ejecuta el siguiente comando que se va a encargar de compilar todo el programa:
    ```
    make
    ```
3. Ejecuta el código:
    ```
    ./bin/image_processing -h
    ```

## Uso

```
Usage: ./image_processing -i <image_name> -o <resulting_image_name> -a <value> -s <value>
Options:
  -h, --help                                      Show this help message
  -v, --version                                   Show program version
  -i, --input_image_name <image_name>             Load specified image
  -o, --output_image_name <resulting_image_name>  Save resulting image with specified name
  -a, --angle <value>                             Rotate image with specified angle
  -xs, --x_scale <value>                          Scale image with specified scale (in X)
  -ys, --y_scale <value>                          Scale image with specified scale (in Y)
  -b, --buddy_system                              Activate buddy system mode (not by default)
```

## Estructura del Proyecto

```
├── include
│   ├── arg_parser.h
│   ├── buddy_allocator.h
│   ├── buddy_image_memory_manager.h
│   ├── IImageMemoryManager.h
│   ├── image.h
│   ├── stb_image.h
│   ├── stb_image_write.h
│   └── vector_image_memory_manager.h
├── Makefile
├── README.md
├── src
│   ├── arg_parser.cpp
│   ├── buddy_allocator.cpp
│   ├── buddy_image_memory_manager.cpp
│   ├── image.cpp
│   ├── main.cpp
│   ├── stb_wrapper.cpp
│   └── vector_image_memory_manager.cpp     
```

## Autores

- Juan Manuel Gómez Piedrahita
- Luisa María Álvarez García
- Miguel Ángel Hoyos
- Santiago Neusa Ruiz
- Sebastián Restrepo Ortiz