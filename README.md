# Dot Product Parallel

This program is used to benchmark different parallel thread policies for computing the
dot product of two vectors v1 and v2 of a given size N.

## Contents

1. [Overview](#overview)
2. [Requirements](#requirements)
3. [Compilation and Usage](#compilation-and-usage)
4. [Known Issues](#known-issues)

## Overview

The program computes the dot product of two vectors which is given by:

$\vec{v_1} \cdot \vec{v_2}$

## Requirements

- g++

## Compilation and Usage

To compile the program simply type:


```shell
make
```

After which you can run the program:

```shell
./dot_product_parallel
```

The program will ask for the user to input the size N of the two vectors v1 and v2:

```shell
Please enter n the size of the two vectors v1 and v2 to perform dot product operation v1.v2:
```

## Results

The following are results for two vectors v1 and v2 of size :

N = 50,000,000 with asynchronous policy

```shell
Enter n the size of the two vectors v1 and v2 to perform dot product operation v1.v2: 50000000          
Using  3 out of 4 hardware threads

Serial dot product = -3458.17
Elapsed time : 372 ms

Packaged tasked based dot product: -3458.35
Elapsed time : 50 ms

Dot Product parallel threads & packaged task: -3458.35
Elapsed time : 51 ms

```

N = 50,000,000 with deferred policy

```shell
Enter n the size of the two vectors v1 and v2 to perform dot product operation v1.v2: 50000000
Using  3 out of 4 hardware threads

Dot product (serial). Answer = -3458.17
Elapsed time : 247 ms

Dot product tasked based: -3458.35
Elapsed time : 206 ms

Dot Product parallel threads & packaged task: -3458.35
Elapsed time : 78 ms
```

N = 100,000,000 with asynchronous policy

```shell
Enter n the size of the two vectors v1 and v2 to perform dot product operation v1.v2: 100000000
Using  3 out of 4 hardware threads

Dot product (serial). Answer = 7781.02
Elapsed time : 2436 ms

Dot product tasked based: 7781.02
Elapsed time : 173 ms

Dot Product parallel threads & packaged task: 7781.02
Elapsed time : 1447 ms
```

N = 100,000,000 with deferred policy

```shell
Enter n the size of the two vectors v1 and v2 to perform dot product operation v1.v2: 100000000
Using  3 out of 4 hardware threads

Dot product (serial). Answer = 7781.02
Elapsed time : 1166 ms

Dot product tasked based: 7781.02
Elapsed time : 169 ms

Dot Product parallel threads & packaged task: 7781.02
Elapsed time : 102 ms
```


## Known Issues

None at present.
