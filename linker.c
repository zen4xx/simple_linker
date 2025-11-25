#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIRST_ARGUMENT 1
#define TEMP "temp"

typedef struct macro_t
{
    char* val;
    char* name;
    struct macro_t* next;
    struct macro_t* prev;
} macro_t;

void add_macro(char* name, char* val, macro_t* prev)
{
    prev->next = malloc(sizeof(macro_t));

    size_t name_size = strlen(name);
    prev->next->name = malloc(name_size * sizeof(char));
    memcpy(prev->next->name, name, name_size);

    size_t val_size = strlen(val);
    prev->next->val = malloc(val_size * sizeof(char));
    memcpy(prev->next->val, val, val_size);

    prev->next->prev = prev;
    prev->next->next = NULL;
}

void delete_macro(macro_t* macro)
{
    macro->prev->next = macro->next;
    free(macro->val);
    free(macro->name);
    free(macro);
}

macro_t first;

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        printf("%s\n", "not enough arguments");
        return 1;
    }

    size_t arg_len = strlen(argv[FIRST_ARGUMENT]);
    char *arg = malloc(arg_len * sizeof(char));
    
    if (arg == NULL)
    {
        printf("%s\n", "failed to allocate arg");
        return 1;
    }

    memcpy(arg, argv[FIRST_ARGUMENT], arg_len);

    FILE* file, *temp;

    file = fopen(arg, "r+");
    temp = fopen(TEMP, "w");

    if (!file || !temp) 
    {
        printf("%s\n", "failed to open file");
        free(arg);
        if (file) fclose(file);
        if (temp) fclose(temp); remove(TEMP);
        return 1;
    }

    
    fclose(file);
    fclose(temp);
    remove(TEMP);
    free(arg);
    return 0;
}
