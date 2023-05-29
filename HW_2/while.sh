#!/bin/bash

# Скрипт, который выводит числа от 1 до 5 с использованием цикла while.

counter=1

# Цикл while для вывода чисел от 1 до 5
while (( counter <= 5 )); do
    echo $counter
    counter=$(( counter + 1 ))
done