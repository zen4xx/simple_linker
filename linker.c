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

    char line[256];
    unsigned int num_quotatation_mark = 0;
    
    macro_t *current_macro = &first;

    while (fgets(line, sizeof(line), file))
    {
        char *c = line;
        while(*c != '\0')
        {
            if(*c == '"')
                ++num_quotatation_mark;
            ++c;
        }
        
        char* def_ptr = strstr(line, "#define");
        if (def_ptr && num_quotatation_mark % 2 == 0)
        { 
            char* name_start = def_ptr + 8; // 8 is "#define" length

            char *val_start = name_start;
            while(*val_start != ' ')
            {
                ++val_start;
                if(*val_start == '\0')
                    goto invalid_macro;
            }
            *val_start = '\0';

            ++val_start;

            char *val_end = val_start;
            while (*val_end != '\n')
            {
                ++val_end;
                if(*val_end == '\0')
                    break;
            }
            *val_end = '\0'; 

            add_macro(name_start, val_start, current_macro);
            current_macro = current_macro->next;
        }
        else 
        {
            // some replacing stuff...
            fprintf(temp, "%s", line);
        }
        invalid_macro:
    }

    macro_t* macro = first.next;

    while (macro != NULL)
    {
        printf("%s", "macro name: ");
        printf("%s\n", macro->name);

        printf("%s", "macro val: ");
        printf("%s\n", macro->val);
        macro = macro->next;
    }
    
    fclose(file);
    fclose(temp);
    //remove(TEMP);
    free(arg);
    return 0;
}
