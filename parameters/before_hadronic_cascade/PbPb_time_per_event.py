#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import numpy as np
import sys

## event number
event = sys.argv[1]

## 'matrix' reads .dat file, and 'squared_matrix' reads the matrix squared
matrix = np.loadtxt(f'{event}.dat', comments='#')
squared_matrix = np.square(matrix)

## initialize with zero
soma = 0
soma_squared = 0

## x e y gets the matrix size
x, y = matrix.shape

## loop on the matrix's lines and columns
for i in range(0, x):
    for j in range(0, y):
        soma = soma + matrix[i][j]
        soma_squared = soma_squared + squared_matrix[i][j]

e_medio = soma_quadrada/soma

## Valores definidos pelo JETSCAPE
tau_r = 1.04
alpha = 0.024
e_r = 4

tau_fs = tau_r * (e_medio/e_r)**alpha

print(tau_fs)
