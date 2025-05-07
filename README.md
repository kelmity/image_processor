# BMP Image Processor

Проект для обработки BMP изображений с поддержкой различных фильтров. Реализует чтение/запись BMP, обработку пикселей и конвейерную фильтрацию.

## Структура проекта
BMP-Image-Processor \
├── CMakeLists.txt # Конфигурация сборки CMake \
├── LICENSE # Лицензия MIT \
├── README.md # Документация \
│ \
├── # Исходные файлы \
│ ├── main.cpp # Точка входа в приложение \
│ ├── parser.cpp # Парсинг аргументов командной строки \
│ ├── parser.h \
│ ├── BMP.cpp # Работа с BMP форматом \
│ ├── BMP.h \
│ ├── filters.cpp # Реализация фильтров \
│ ├── filters.h \
│ ├── filter_processing.cpp # Применение фильтров к изображению \
│ └── filter_processing.h \
│ \
└── tests/ # Юнит-тесты \
├── image_processing # Тесты обработки изображений \
└── test_scripts/ # Скрипты для тестирования \

## Основные компоненты

### 1. Чтение/запись BMP
- Поддержка 24-битных BMP файлов
- Валидация заголовков файлов
- Обработка выравнивания строк

```cpp
// Пример использования:
BMPFile bmp("input.bmp");
Image img = bmp.read();
bmp.write("output.bmp", img);
```

2. Система фильтров

    Модульная архитектура фильтров

    Поддержка цепочки фильтров

    Быстрое применение преобразований
// Пример фильтрации:
apply_filter(img, FilterType::GRAYSCALE);
apply_filter(img, FilterType::CROP, {800, 600});
3. Парсинг аргументов

    Обработка параметров командной строки

    Валидация входных значений

    Генерация сообщений об ошибках

# Пример запуска:
./image_processor input.bmp output.bmp \
    -crop 800 600 \
    -blur 5.2 \
    -edge 0.75

Сборка проекта

Требования:

    C++17

    CMake ≥3.10

    Компилятор с поддержкой C++17

# Сборка через CMake:
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make

# Запуск тестов:
ctest --verbose

# Пример использования:
./image_processor input.bmp output.bmp -neg -sharp

Поддерживаемые фильтры
Фильтр	Параметры	Описание
-crop	width height	Обрезка изображения
-gs	–	Градации серого
-neg	–	Цветовая инверсия
-sharp	–	Повышение резкости
