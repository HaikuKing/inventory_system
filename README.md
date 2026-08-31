# Program Explanation
This program is intended to be an inventory, or digital storage, system. You should be able to sort by multiple factors. These optional filters should also be reversible.

# Installation
1. The first thing to do is to open a terminal and designatea spot for this program.

2. You will need to clone this repository.
```git clone https://github.com/HaikuKing/inventory_system```

3. Enter the inventory_system folder that was just created.
```cd inventory_system```
4. Alter the enum, union, and struct at the top of the main.c file to accomodate any filters and variables you would like.
**Caution: You may have to alter, add, or remove lines of code from the functions at the bottom of the file as well to make this work. I haven't tested this yet.**
5. Compile the main.c file and name the output file. 
```gcc ./main.c -o execute```
6. Run the program by using the named output. For example:
```./execute```
7. The contents of the inventory will be saved to, and loaded from, "inventory.dat" in the program folder.

# Contact
If you find any issues, don't hesitate to contact me. You can find my e-mail at https://haikuking.github.io/static_site_gen/ in the "Contact Information" section.

Thank you for taking interest in this project of mine - HaikuKing
