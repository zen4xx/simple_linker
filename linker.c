#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIRST_ARGUMENT 1
#define TEMP "temp"

typedef struct macro_t
{
    char* val;
    struct macro_t* next;
    struct macro_t* prev;
} macro_t;

void add_macro(char* val, macro_t* macro)
{
    macro->next = malloc(sizeof(macro_t));
    size_t val_size = strlen(val);
    macro->next->val = malloc(val_size * sizeof(char));
    memcpy(macro->next->val, val, val_size);
    macro->next->prev = macro;
    macro->next->next = NULL;
}

void delete_macro(macro_t* macro)
{
    macro->prev->next = macro->next;
    free(macro->val);
    free(macro);
}


int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        printf("%s\n", "too many arguments");
        return 1;
    }

    size_t arg_len = strlen(argv[FIRST_ARGUMENT]);
    char *arg = malloc(arg_len * sizeof(char));
    
    if (arg == nullptr)
    {
        printf("%s\n", "failed to allocate arg");
        return 1;
    }

    memcpy(arg, argv[FIRST_ARGUMENT], arg_len);

    FILE* file, *temp;

    file = fopen(arg, "r");
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
