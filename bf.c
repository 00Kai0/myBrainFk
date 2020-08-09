#include<stdio.h>
#include<stdlib.h>

typedef enum node_type {
    HEAD=1,
    POINTER_ADD,
    POINTER_SUB,
    BYTE_ADD,
    BYTE_SUB,
    OUTPUT,
    INPUT,
    START_LOOP,
    END_LOOP
} node_type;

typedef struct node {
    struct node* next;
    node_type type;
} node;

typedef struct proc_state {
    node*  c_node;    // current node 
    node*  head_node; // head node
    node** stack;     // loop stack
    char*  data;      // memory 
    size_t stack_top; // stack top
    size_t pointer;   // currently data pointer
    size_t ssize;     // stack size, 200 by default
    size_t dsize;     // memory size, 200 by default
} proc_state;

node*
new_node(node* nodep, node_type type)
{
    nodep->next = (node*)malloc(sizeof(node));
    nodep = nodep->next;
    nodep->next = NULL;
    nodep->type = type;
}

# define NEXT_NODE(ps) ps->c_node = ps->c_node->next;
# define DEFAULT_SIZE 200

void
init_proc_state(proc_state* p)
{
    p->c_node = (node*)malloc(sizeof(node));
    p->c_node->type = HEAD;
    p->c_node->next = NULL;
    p->head_node = p->c_node;
    p->stack = (node**)malloc(sizeof(node*)*200);
    p->data = (char*)malloc(sizeof(char)*200);
    p->dsize = DEFAULT_SIZE;
    p->ssize = DEFAULT_SIZE;
    p->pointer = 0;
    p->stack_top = 0;

    // Init data
    for (int i=0;i<p->dsize;i++) {
        p->data[i] = 0;
    }
}

/* **DEBUG** Show Node List*/
void
print_nodes(proc_state* p)
{
    node* nodep = p->head_node;
    while (nodep) {
        printf("type: %d ", nodep->type);
        switch (nodep->type) {
            case HEAD:
                printf("HEAD\n");
                break;
            case POINTER_ADD: 
                printf("POINTER_ADD\n");
                break;
            case POINTER_SUB:
                printf("POINTER_SUB\n");
                break;
            case BYTE_ADD:
                printf("BYTE_ADD\n");
                break;
            case BYTE_SUB:
                printf("BYTE_SUB\n");
                break;
            case OUTPUT:
                printf("OUTPUT\n");
                break;
            case INPUT:
                printf("INPUT\n");
                break;
            case START_LOOP:
                printf("START_LOOP\n");
                break;
            case END_LOOP:
                printf("END_LOOP\n");
                break;
            default:
                fprintf(stderr, "unexceped option\n");
        }
        nodep = nodep->next;

    }
}

/* Run Programme */
void
run_programme(proc_state* ps)
{
    while (ps->c_node) {
        switch (ps->c_node->type) {
            case HEAD:
                /* NULL */
                NEXT_NODE(ps);
                break;
            case POINTER_ADD: 
                if (ps->pointer >= ps->dsize - 1) {
                    fprintf(stderr, "Data OverFlow\n");
                    goto EXIT;
                }
                // unclear data
                //ps->data[++ps->pointer] = 0;
                ++ps->pointer;
                NEXT_NODE(ps);
                break;
            case POINTER_SUB:
                if (ps->pointer <= 0) {
                    fprintf(stderr, "Data UnderFlow\n");
                    goto EXIT;
                }
                --ps->pointer;
                NEXT_NODE(ps);
                break;
            case BYTE_ADD:
                ++ps->data[ps->pointer];
                NEXT_NODE(ps);
                break;
            case BYTE_SUB:
                --ps->data[ps->pointer];
                NEXT_NODE(ps);
                break;
            case OUTPUT:
                putc(ps->data[ps->pointer], stdout);
                NEXT_NODE(ps);
                break;
            case INPUT:
                ps->data[ps->pointer] = getc(stdin);
                NEXT_NODE(ps);
                break;
            case START_LOOP:
                if (ps->data[ps->pointer] != 0) {
                    /* enable loop */
                    if (ps->stack_top >= ps->ssize) {
                        fprintf(stderr, "Stack OverFlow\n");
                        goto EXIT;
                    }
                    ps->stack[ps->stack_top++] = ps->c_node;
                } else {
                    /* end loop */
                    while (ps->c_node->type != END_LOOP) {
                        NEXT_NODE(ps);
                        if (!ps->c_node) {
                            fprintf(stderr, "Unexcepted Loop\n");
                            goto EXIT;
                        }
                    }
                }
                NEXT_NODE(ps);
                break;
            case END_LOOP:
                if (ps->stack_top<=0) {
                    fprintf(stderr, "Stack UnderFlow\n");
                    goto EXIT;
                }

                if (ps->data[ps->pointer] != 0) {
                    /* loop continue*/
                    ps->c_node = ps->stack[ps->stack_top-1]->next;
                } else {
                    /* stop loop*/
                    ps->stack[--ps->stack_top] = NULL;
                    NEXT_NODE(ps);
                }
                break;
            default:
                fprintf(stderr, "unexceped option\n");
                goto EXIT;
        }
    }

EXIT:
    return;

}


int
main(int argc, const char** argv)
{
    if (argc < 2) {
        fprintf(stderr, "Miss File.\n");
        return 1;
    }
    else if (argc > 2) {
        fprintf(stderr, "Unexceped arguments.\n");
        return 1;
    }

    const char* filename = argv[1];
    // printf("Loading File: %s...\n", filename);

    proc_state global_proc;
    init_proc_state(&global_proc);

    FILE* fp;
    fp = fopen(filename, "r");

    if (fp==NULL) {
        fprintf(stderr, "File does not exist.\n");
        return 1;
    }

    // parse code
    node* nodep = global_proc.c_node;
    for (;;) {
        char c = fgetc(fp);
        if (feof(fp)) {
            break;
        }

        switch (c) {
            case '>': nodep = new_node(nodep, POINTER_ADD);break;
            case '<': nodep = new_node(nodep, POINTER_SUB);break;
            case '+': nodep = new_node(nodep, BYTE_ADD);break;
            case '-': nodep = new_node(nodep, BYTE_SUB);break;
            case '.': nodep = new_node(nodep, OUTPUT);break;
            case ',': nodep = new_node(nodep, INPUT);break;
            case '[': nodep = new_node(nodep, START_LOOP);break;
            case ']': nodep = new_node(nodep, END_LOOP);break;
            default: continue;
        }
    }

    fclose(fp);

    // Debug print
    //print_nodes(&global_proc);

    // Run Programme
    run_programme(&global_proc);

    return 0;

ERROR:
    fclose(fp);
    return 1;
}
