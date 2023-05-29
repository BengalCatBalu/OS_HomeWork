#!/bin/bash

# Функция для вычисления суммы двух чисел
function sum() {
    local a=$1
    local b=$2
    local result=$(( a + b ))
    echo "Сумма чисел $a и $b равна $result"
}

# Вызов функции sum с аргументами 5 и 3
sum 5 3