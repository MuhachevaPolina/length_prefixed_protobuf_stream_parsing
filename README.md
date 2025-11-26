# 102-cpp-protobuf-pmuhacheva

Репозиторий задания "Разбор потока length-prefixed Protobuf сообщений на C++" (LEARNING_CENTER-102)

Выполнила: Мухачева Полина

## Запуск
### Из bash-скрипта
В корне проекта выполнить команду
```
bash start.sh
```
### Вручную
1. Удалить директорию build/, если она уже существует
```
rm -rf build
```
2. Создать build/ и запустить из нее сборку проекта
```
mkdir build
cd build
cmake ..
make
```

3. Запустить проект
```
./test/test_protobuf_parser
```

### Запуск ctset
После сборки из директории build/ выполнить команду
```
ctset
```