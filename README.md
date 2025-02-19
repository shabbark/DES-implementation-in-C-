# Data Encryption Standard (DES) - 2 Round Implementation in C++

## Overview

This project implements a **2-round** version of the **Data Encryption Standard (DES)** in **C++**. It demonstrates how the DES encryption process works by performing **key generation, expansion, XOR operations, S-Box compression, and Feistel structure-based transformation**.  

## Features

- Converts **8-character plaintext** into **64-bit binary**.  
- Converts **8-character key** into **64-bit binary**, removing parity bits to form a **56-bit key**.  
- Performs **two rounds** of DES encryption, including:  
  - **Key scheduling** (shifting and reducing to 48 bits).  
  - **Expansion, XOR, and compression** of plaintext halves.  
  - **Final ciphertext output** after two rounds.  
- **Detailed logging** to track each step of encryption.  

## How It Works  

### **1. Key Scheduling**  
- The **64-bit key** undergoes a **Parity Drop** to become **56 bits**.  
- It is **split into two 28-bit halves**.  
- **Round 1:**
  - Both halves are **left-shifted once** and then merged into a **48-bit round key**.  
- **Round 2:**
  - The original halves are **left-shifted twice**, merged, and reduced to another **48-bit round key**.  

### **2. Encryption Rounds**  
- **Plaintext is split** into two **32-bit halves**.  
- **Round 1:**
  - The **right half** is expanded to **48 bits** using an **Expansion P-Box**.  
  - XOR operation with **48-bit round key**.  
  - Compressed back to **32 bits** using an **S-Box**.  
  - XOR with the **left half** to generate a new **right half**.  
  - The original right half becomes the **left half** for the next round.  
- **Round 2:**  
  - The same process repeats using the second **48-bit round key**.  
  - The final **ciphertext** is generated.  
