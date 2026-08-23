# C Programming Practices

Four standalone C programs from university coursework, each one a complete console application built around a specific set of concepts: structs, validation, dynamic memory, file handling and linked lists.

This repository is private. It is shared for portfolio purposes only, and access is limited to people who have been explicitly invited.

## Practices

### 1.1 BMI Tracker

A console program for a pediatric clinic to track children's height, weight and BMI, classifying them into weight categories and estimating future weight gain. Includes input validation and an activity diagram.

### 1.2 Medical Visits

A system for managing doctors and medical visits using structs, with validation of doctor availability, vacation months and visit dates.

### 2.1 Spam Classifier

Loads a training dataset of emails, filters them by category, calculates prediction metrics, and can classify new emails as spam or not through manual prediction.

### 2.2 Ski Rental (LS Ski)

An inventory and rental management system for a ski shop: skis, poles, boots and helmets. Supports checking inventory, preparing item packs and booking them for the coming season. Uses dynamic arrays and a linked list to manage the packs.

`linkedlist.h` in this folder is the linked list implementation provided by the course teaching staff, not my own code. My part is `main.c`, which uses it to build the booking system.

## Tech stack

- C
- Structs, dynamic memory, file handling, string parsing and a linked list data structure

## Running any of them

Each folder is a single, self-contained `main.c` file. Compile it with any standard C compiler, for example:
