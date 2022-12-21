/*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
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
    *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    bst_node_t *tmp = tree;

    if(!tmp){
        return false;
    }

    if(tmp->key == key){
        *value = tmp->value;
        return true;
    }
    else if(tmp->key > key){
        if(tmp->left != NULL){
            bst_search(tmp->left, key, value);
        }
        else{
            return false;
        }
    }
    else if(tmp->key < key){
        if(tmp->right != NULL){
            bst_search(tmp->right, key, value);
        }
        else{
            return false;
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    bst_node_t *tmp = *tree;
    bst_node_t *insert = malloc(sizeof(bst_node_t));

    insert->left = NULL;
    insert->right = NULL;
    insert->key = key;
    insert->value = value;

    if(!tmp){
        *tree = insert;
        return;
    }

    if(tmp->key == key){
        tmp->value = value;
        free(insert);
        return;
    }
    else if(tmp->key > key){
        if(tmp->left != NULL){
            free(insert);
            bst_insert(&tmp->left, key, value);
        }
        else{
            tmp->left = insert;
            return;
        }
    }
    else if(tmp->key < key){
        if(tmp->right != NULL){
            free(insert);
            bst_insert(&tmp->right, key, value);
        }
        else{
        tmp->right = insert;
        return;
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
    bst_node_t *tmp = *tree;
    bst_node_t *previous = target;

    if(!tmp){
        return;
    }

    if(tmp->right->right){
        bst_replace_by_rightmost(target, &tmp->right);
        return;
    }
    if(tmp->right){
        if(tmp->right->left == NULL){
            previous = tmp;
            tmp = tmp->right;
            target->value = tmp->value;
            target->key = tmp->key;
            free(tmp);
            previous->right = NULL;
            return;
        }
        else{
            previous = tmp;
            tmp = tmp->right;
            target->value = tmp->value;
            target->key = tmp->key;
            previous->right = tmp->left;
            free(tmp);
        }
    }
    else{
        target->value = tmp->value;
        target->key = tmp->key;
        if(tmp->left){
            previous->right = tmp->left;
        }
        else{
            previous->right = NULL;
        }
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
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {

    if(!(*tree)){
        return;
    }
    if((*tree)->key == key) { //the current key is the one we are looking for
        if (!(*tree)->left && !(*tree)->right) {
            free((*tree));
            (*tree) = NULL;
        }
        else if ((!(*tree)->left && (*tree)->right) || ((*tree)->left && !(*tree)->right)) {
            if ((*tree)->left) {
                bst_node_t *tmpDel = (*tree);
                (*tree) = (*tree)->left;
                free(tmpDel);
            } else {
                bst_node_t *tmpDel = *tree;
                (*tree) = (*tree)->right;
                free(tmpDel);
            }
        }
        else if ((*tree)->left && (*tree)->right) {
            bst_replace_by_rightmost((*tree), &(*tree)->left);
            return;
        }
    }
    else if((*tree)->key > key) {
        if ((*tree)->left != NULL) {
            bst_delete(&(*tree)->left, key);
            return;
        } else {
            return;
        }
    }
    else if((*tree)->key < key){
        if((*tree)->right != NULL){
            bst_delete(&(*tree)->right, key);
            return;
        }
        else{
            return;
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
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
    if(*tree != NULL){
        bst_dispose(&(*tree)->left);
        bst_dispose(&(*tree)->right);
        free(*tree);
        (*tree) = NULL;
    }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
    bst_node_t *tmp = tree;

    if(tmp){
        bst_print_node(tmp);
        bst_preorder(tmp->left);
        bst_preorder(tmp->right);
    }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
    bst_node_t *tmp = tree;

    if(tmp){
        bst_inorder(tmp->left);
        bst_print_node(tmp);
        bst_inorder(tmp->right);
    }
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
    bst_node_t *tmp = tree;

    if(tmp){
        bst_postorder(tmp->left);
        bst_postorder(tmp->right);
        bst_print_node(tmp);
    }
}
