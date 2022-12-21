/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
    (*tree) = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    bst_node_t *tmp = tree;
    while(tmp) {
        if(tmp->key == key){
            *value = tmp ->value;
            return true;
        }
        else if(tmp->key > key){
            if(tmp->left != NULL){
                tmp = tmp->left;
            }
            else{
                return false;
            }
        }
        else{
            if(tmp->right != NULL){
                tmp = tmp->right;
            }
            else{
                return false;
            }
        }
    }
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {

    bst_node_t  *treePtr = *tree;

    bst_node_t *insert = malloc(sizeof(struct bst_node));
    insert -> key = key;
    insert -> value = value;
    insert -> left = NULL;
    insert -> right = NULL;

    if(treePtr == NULL){
        *tree = insert;
        return;
    }
    //if we are on the key we want to change
    while(treePtr){
        if(treePtr->key == key){
            treePtr->value = value;
            free(insert);
            break;
        }
        else if(treePtr->key > key){
            if(treePtr->left != NULL){
                treePtr = treePtr->left;
            }
            else{
                treePtr->left = insert;
                break;
            }
        }
        else if(treePtr->key < key){
            if(treePtr->right != NULL){
                treePtr = treePtr->right;
            }
            else{
                treePtr->right = insert;
                break;
            }
        }
    }
}
/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    bst_node_t *tmp = *tree;
    bst_node_t *previous = target;

    if(!tmp){
        return;
    }
    while(tmp->right){
        previous = tmp;
        tmp = tmp->right;
    }
    if(tmp->left == NULL){
        target->value = tmp->value;
        target->key = tmp->key;
        if(target != previous){
            previous->right = NULL;
        }
        else{
            previous->left = NULL;
        }
        free(tmp);
    }
    else{
        target->value = tmp->value;
        target->key = tmp->key;
        previous->right = tmp->left;
        free(tmp);
    }
}
/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
    bst_node_t *tmp = *tree;
    bst_node_t *previous = NULL;

    while(tmp){
        if(tmp->key == key){
            if(!tmp->left && !tmp->right){
                if(tmp == *tree){
                    free(tmp);
                    return;
                }
                if(previous->left == tmp){
                    previous->left = NULL;
                }
                if(previous->right == tmp){
                    previous->right = NULL;
                }
                free(tmp);
                break;
            }
            else if((!tmp->left && tmp->right) || (tmp->left && !tmp->right)){
                if(previous != NULL){
                    if(previous->left == tmp){
                        if(tmp->left != NULL){
                            previous->left = tmp->left;
                        }
                        else if(tmp->right != NULL){
                            previous->left = tmp->right;
                        }
                    }
                    else if(previous->right == tmp){
                        if(tmp->left != NULL){
                            previous->right = tmp->left;
                        }
                        else if(tmp->right != NULL){
                            previous->right = tmp->right;
                        }
                    }
                    free(tmp);
                    break;
                }
            }
            else if(tmp->left && tmp->right){
                bst_replace_by_rightmost(tmp, &(tmp->left));
            }
        }
        if(tmp->key > key){
            if(tmp->left != NULL){
                previous = tmp;
                tmp = tmp->left;
            }
            else{
                break;
            }
        }
        else{
            if(tmp->right != NULL){
                previous = tmp;
                tmp = tmp->right;
            }
            else{
                break;
            }
        }
    }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
    stack_bst_t stack;
    stack_bst_init(&stack);
    stack_bst_push(&stack, *tree);

    while(!stack_bst_empty(&stack)){
        bst_node_t *tmp = stack_bst_pop(&stack);
        if(!tmp){
            continue;
        }
        stack_bst_push(&stack, tmp->left);
        stack_bst_push(&stack, tmp->right);

        free(tmp);
    }
    *tree = NULL;
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
    bst_node_t *tmp = tree;

    while(tmp){
        stack_bst_push(to_visit, tmp);
        bst_print_node(tmp);
        tmp = tmp->left;
    }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
    stack_bst_t stack;
    stack_bst_init(&stack);
    bst_node_t *tmp = tree;

    bst_leftmost_preorder(tmp, &stack);

    while(!stack_bst_empty(&stack)){
        tmp = stack_bst_top(&stack);
        stack_bst_pop(&stack);
        bst_leftmost_preorder(tmp->right, &stack);
    }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {

    bst_node_t *tmp = tree;

    while(tmp){
            stack_bst_push(to_visit, tmp);
            tmp = tmp->left;
        }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
    bst_node_t *tmp = tree;
    stack_bst_t stack;
    stack_bst_init(&stack);
    bst_leftmost_inorder(tmp, &stack);

    while(!stack_bst_empty(&stack)){
        tmp = stack_bst_top(&stack);
        stack_bst_pop(&stack);
        bst_print_node(tmp);
        bst_leftmost_inorder(tmp->right, &stack);
    }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
    bst_node_t *tmp = tree;

    while(tmp){
        stack_bst_push(to_visit, tmp);
        stack_bool_push(first_visit, true);
        tmp = tmp->left;
    }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
    bst_node_t *tmp = tree;
    stack_bst_t stack;
    stack_bst_init(&stack);
    stack_bool_t stackBool;
    stack_bool_init(&stackBool);
    bool left;
    bst_leftmost_postorder(tmp, &stack, &stackBool);

    while(!stack_bst_empty(&stack)){
        tmp = stack_bst_top(&stack);
        stack_bst_pop(&stack);
        left = stack_bool_pop(&stackBool);

        if(left){
            stack_bst_push(&stack, tmp);
            stack_bool_push(&stackBool, false);
            bst_leftmost_postorder(tmp->right, &stack, &stackBool);
        }
        else{
            bst_print_node(tmp);
        }

    }

}
