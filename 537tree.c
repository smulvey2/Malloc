#include<stdio.h>
#include<stdlib.h>
#include "537malloc.h"
#include "537tree.h"

Node* newNode(tuple data) {
	Node *temp   = (Node*) malloc(sizeof(Node));
    temp->data   = data;
    temp->color  = 'R';
    temp->left   = NULL;
    temp->right  = NULL;
    temp->parent = NULL;

    return temp;
}

// left rotates the given node 
void leftRotate(tree* maple, Node *x) { 
	// new parent will be node's right child 
	Node *nParent = x->right; 

	// update root if current node is root 
	if (x == maple->root) 
	maple->root = nParent; 

	//moveDown(x, nParent); 
	if (x->parent != NULL) { 
		if (isOnLeft(x) == 1) { 
			x->parent->left = nParent; 
		} else { 
			x->parent->right = nParent; 
		} 
    } 
    nParent->parent = x->parent; 
    x->parent = nParent;

	// connect x with new parent's left element 
	x->right = nParent->left; 
	// connect new parent's left element with node 
	// if it is not null 
	if (nParent->left != NULL) 
	nParent->left->parent = x; 

	// connect new parent with x 
	nParent->left = x; 
} 

void rightRotate(tree* maple, Node *x) { 
	// new parent will be node's left child 
	Node *nParent = x->left; 

	// update root if current node is root 
	if (x == maple->root) 
	maple->root = nParent; 

	//moveDown(x, nParent); 
	if (x->parent != NULL) { 
		if (isOnLeft(x) == 1) { 
			x->parent->left = nParent; 
		} else { 
			x->parent->right = nParent; 
		} 
    } 
    nParent->parent = x->parent; 
    x->parent = nParent;

	// connect x with new parent's right element 
	x->left = nParent->right; 
	// connect new parent's right element with node 
	// if it is not null 
	if (nParent->right != NULL) 
	nParent->right->parent = x; 

	// connect new parent with x 
	nParent->right = x; 
} 

// moves node down and moves given node in its place 
void moveDown(Node* x, Node *nParent) { 
	if (x->parent != NULL) { 
		if (isOnLeft(x) == 1) { 
			x->parent->left = nParent; 
		} else { 
			x->parent->right = nParent; 
		} 
    } 
    nParent->parent = x->parent; 
    x->parent = nParent; 
} 

void swapColors(Node *x1, Node *x2) { 
	Node* temp = NULL; 
	temp->color = x1->color; 
	x1->color = x2->color; 
	x2->color = temp->color; 
} 

void swapValues(Node *u, Node *v) { 
	tuple temp; 
	temp = u->data; 
	u->data = v->data; 
	v->data = temp; 
} 

int isOnLeft(Node* pt) { 
	if (pt == pt->parent->left) {
		return 1;
	}
	return 0; 
}

// fix red red at given node 
void fixRedRed(tree* maple, Node *x) { 
	// if x is root color it black and return 
	if (x == maple->root) { 
	x->color = 'B'; 
	return; 
	} 

	// initialize parent, grandparent, uncle 
	Node *parent = x->parent, *grandparent = parent->parent;
	Node *uncle;
	if (x->parent->left == x) 
		uncle = x->parent->parent->right;
    else 
		uncle = x->parent->parent->left; 

	if (parent->color != 'B') {
	if (uncle != NULL) {
	if (uncle->color == 'R') { 
		// uncle red, perform recoloring and recurse 
		parent->color = 'B'; 
		uncle->color = 'B'; 
		grandparent->color = 'R'; 
		fixRedRed(maple, grandparent);
	}
	} else { 
		// Else perform LR, LL, RL, RR 
		if (isOnLeft(parent) == 1) { 
		if (isOnLeft(x) == 1) { 
			// for left right 
			swapColors(parent, grandparent); 
		} else { 
			leftRotate(maple, parent);
			swapColors(x, grandparent); 
		} 
		// for left left and left right 
		rightRotate(maple, grandparent); 
		} else { 
		if (isOnLeft(x) == 1) { 
			// for right left 
			rightRotate(maple, parent);
			swapColors(x, grandparent); 
		} else { 
			swapColors(parent, grandparent); 
		} 

		// for right right and right left 
		leftRotate(maple, grandparent);
		} 
	} 
	} 
} 

// find node that do not have a left child 
// in the subtree of the given node 
Node *successor(Node *x) { 
	Node *temp = x; 

	while (temp->left != NULL) 
	temp = temp->left; 

	return temp; 
} 

// find node that replaces a deleted node in BST 
Node *BSTreplace(Node *x) { 
	// when node have 2 children 
	if ((x->left != NULL) && (x->right != NULL)) 
	return successor(x->right); 

	// when leaf 
	if ((x->left == NULL) && (x->right == NULL)) 
	return NULL; 

	// when single child 
	if (x->left != NULL) 
	return x->left; 
	else
	return x->right; 
} 

// returns pointer to sibling 
Node *sib(Node* pt) { 
    // sibling null if no parent 
    if (pt->parent == NULL) 
      return NULL; 
  
    if (isOnLeft(pt) == 1) 
		return pt->parent->right; 
  
    return pt->parent->left; 
  } 

// deletes the given node 
void deleteNode(tree* maple, Node *v) { 
	Node *u = BSTreplace(v); 
	if (u == v) {
		u = NULL;
	}
	int uvBlack = 0;
	// True when u and v are both black 
	if (u != NULL) {
	if ((u->color == 'B') && (v->color == 'B')) { 
		uvBlack = 1;
	}
	}
	Node *parent = v->parent; 

	if (u == NULL) { 
	// u is NULL therefore v is leaf 
	if (v == maple->root) { 
		// v is root, making root null 
		maple->root = NULL; 
	} else { 
		if (uvBlack == 1) { 
		// u and v both black 
		// v is leaf, fix double black at v 
		fixDoubleBlack(maple, v); 
		} else { 
		// u or v is red 
		if (sib(v) != NULL) 
			// sibling is not null, make it red" 
			sib(v)->color = 'R'; 
		} 

		// delete v from the tree 
		if (isOnLeft(v) == 1) { 
		parent->left = NULL; 
		} else { 
		parent->right = NULL; 
		} 
	} 
	free(v);
	v = NULL;
	return; 
	} 

	if ((v->left == NULL) || (v->right == NULL)) { 
	// v has 1 child 
	if (v == maple->root) { 
		// v is root, assign the value of u to v, and delete u 
		v->data = u->data; 
		v->left = v->right = NULL; 
		free(u); 
		u = NULL;
	} else { 
		// Detach v from tree and move u up 
		if (isOnLeft(v) == 1) { 
		parent->left = u; 
		} else { 
		parent->right = u; 
		} 
		free(v); 
		v = NULL;
		u->parent = parent; 
		if (uvBlack) { 
		// u and v both black, fix double black at u 
		fixDoubleBlack(maple, u); 
		} else { 
		// u or v red, color u black 
		u->color = 'B'; 
		} 
	} 
	return; 
	} 

	// v has 2 children, swap values with successor and recurse 
	swapValues(u, v); 
	deleteNode(maple, u); 
} 

int hasRedChild(Node* pt) { 
	if ((pt->left != NULL) || (pt->right != NULL)) {
    if ((pt->left->color == 'R') || (pt->right->color == 'R')) { 
		return 1;
	}
	}
	return 0;
} 

void fixDoubleBlack(tree* maple, Node *x) { 
	if (x == maple->root) 
	// Reached root 
	return; 

	Node *sibling;
	sibling = sib(x);
	Node *parent = x->parent; 
	if (sibling == NULL) { 
	// No sibiling, double black pushed up 
	fixDoubleBlack(maple, parent); 
	} else { 
	if (sibling->color == 'R') { 
		// Sibling red 
		parent->color = 'R'; 
		sibling->color = 'B'; 
		if (isOnLeft(sibling) == 1) { 
		// left case 
		rightRotate(maple, parent); 
		} else { 
		// right case 
		leftRotate(maple, parent); 
		} 
		fixDoubleBlack(maple, x); 
	} else { 
		// Sibling black 
		if (hasRedChild(sibling) == 1) { 
		// at least 1 red children 
		if (sibling->left != NULL) {
		if (sibling->left->color == 'R') { 
			if (isOnLeft(sibling) == 1) { 
			// left left 
			sibling->left->color = sibling->color; 
			sibling->color = parent->color; 
			rightRotate(maple, parent); 
			} else { 
			// right left 
			sibling->left->color = parent->color; 
			rightRotate(maple, sibling); 
			leftRotate(maple, parent); 
			} 
		}
		} else { 
			if (isOnLeft(sibling) == 1) { 
			// left right 
			sibling->right->color = parent->color; 
			leftRotate(maple, sibling); 
			rightRotate(maple, parent);
			} else { 
			// right right 
			sibling->right->color = sibling->color; 
			sibling->color = parent->color; 
			leftRotate(maple, parent);
			} 
		} 
		parent->color = 'B'; 
		} else { 
		// 2 black children 
		sibling->color = 'R'; 
		if (parent->color == 'B') 
			fixDoubleBlack(maple, parent); 
		else
			parent->color = 'B'; 
		} 
	} 
	} 
} 

// utility function that deletes the node with given value 
void deleteByVal(tree* maple, tuple n) { 
	if (maple->root == NULL) 
	// Tree is empty 
	return; 

	Node *v = search(maple, n);
	
	if (v == NULL) { 
	printf("No node found to delete with value."); 
	return; 
	} 

	deleteNode(maple, v); 
} 

/* A utility function to insert a new node with given key 
   in BST */
Node* BSTInsert(Node *root, Node *pt) 
{ 
    /* If the tree is empty, return a new node */
    if (root == NULL) 
		return pt; 
  
    /* Otherwise, recur down the tree */
    if (pt->data.addr < root->data.addr) 
    { 
        root->left = BSTInsert(root->left, pt); 
        root->left->parent = root; 
    } 
    else if (pt->data.addr > root->data.addr) 
    { 
        root->right = BSTInsert(root->right, pt); 
        root->right->parent = root; 
    } 
  
    /* return the (unchanged) node pointer */
    return root; 
} 

void rotateLeft(tree* maple, Node *pt) 
{ 
    Node *pt_right = pt->right; 
  
    pt->right = pt_right->left; 
  
    if (pt->right != NULL) 
        pt->right->parent = pt; 
  
    pt_right->parent = pt->parent; 
  
    if (pt->parent == NULL) 
        maple->root = pt_right; 
  
    else if (pt->data.addr == pt->parent->left->data.addr) 
        pt->parent->left = pt_right; 
  
    else
        pt->parent->right = pt_right; 
  
    pt_right->left = pt; 
    pt->parent = pt_right; 
} 

void rotateRight(tree* maple, Node *pt) 
{ 
    Node *pt_left = pt->left; 
  
    pt->left = pt_left->right; 
  
    if (pt->left != NULL) 
        pt->left->parent = pt; 
  
    pt_left->parent = pt->parent; 
  
    if (pt->parent == NULL) 
        maple->root = pt_left; 
  
    else if (pt->data.addr == pt->parent->left->data.addr) 
        pt->parent->left = pt_left; 
  
    else
        pt->parent->right = pt_left; 
  
    pt_left->right = pt; 
    pt->parent = pt_left; 
} 

// This function fixes violations caused by BST insertion 
void fixViolation(tree* maple, Node *pt) 
{ 
    Node *parent_pt = NULL; 
    Node *grand_parent_pt = NULL; 
  
    while ((pt != maple->root) && (pt->color != 'B') && 
           (pt->parent->color == 'R')) 
    { 
  
        parent_pt = pt->parent; 
        grand_parent_pt = pt->parent->parent; 
  
        /*  Case : A 
            Parent of pt is left child of Grand-parent of pt */
        if (parent_pt == grand_parent_pt->left) 
        { 
  
            Node *uncle_pt = grand_parent_pt->right; 
  
            /* Case : 1 
               The uncle of pt is also red 
               Only Recoloring required */
            if (uncle_pt != NULL && uncle_pt->color == 'R') 
            { 
                grand_parent_pt->color = 'R'; 
                parent_pt->color = 'B'; 
                uncle_pt->color = 'B'; 
                pt = grand_parent_pt; 
            } 
  
            else
            { 
                /* Case : 2 
                   pt is right child of its parent 
                   Left-rotation required */
                if (pt == parent_pt->right) 
                { 
                    rotateLeft(maple, parent_pt); 
                    pt = parent_pt; 
                    parent_pt = pt->parent; 
                } 
  
                /* Case : 3 
                   pt is left child of its parent 
                   Right-rotation required */
                rotateRight(maple, grand_parent_pt); 
				char temp = parent_pt->color;
				parent_pt->color = grand_parent_pt->color;
				grand_parent_pt->color = temp;
                pt = parent_pt; 
            } 
        } 
  
        /* Case : B 
           Parent of pt is right child of Grand-parent of pt */
        else
        { 
            Node *uncle_pt = grand_parent_pt->left; 
  
            /*  Case : 1 
                The uncle of pt is also red 
                Only Recoloring required */
            if ((uncle_pt != NULL) && (uncle_pt->color == 'R')) 
            { 
                grand_parent_pt->color = 'R'; 
                parent_pt->color = 'B'; 
                uncle_pt->color = 'B'; 
                pt = grand_parent_pt; 
            } 
            else
            { 
                /* Case : 2 
                   pt is left child of its parent 
                   Right-rotation required */
                if (pt == parent_pt->left) 
                { 
                    rotateRight(maple, parent_pt); 
                    pt = parent_pt; 
                    parent_pt = pt->parent; 
                } 
  
                /* Case : 3 
                   pt is right child of its parent 
                   Left-rotation required */
                rotateLeft(maple, grand_parent_pt); 
				char temp = parent_pt->color;
				parent_pt->color = grand_parent_pt->color;
				grand_parent_pt->color = temp;
                pt = parent_pt; 
            } 
        } 
    } 
  
    maple->root->color = 'B'; 
} 
  
// Function to insert a new node with given data 
void insert(tree* maple, tuple data) 
{ 
    Node *pt = newNode(data); 
  
    // Do a normal BST insert 
    maple->root = BSTInsert(maple->root, pt); 
  
    // fix Red Black Tree violations 
    fixViolation(maple, pt); 
} 


#define COUNT 10 

// Function to print binary tree in 2D 
// It does reverse inorder traversal 
void print2DUtil(Node* root, int space) 
{ 
    // Base case 
    if (root == NULL) 
        return; 
  
    // Increase distance between levels 
    space += COUNT; 
  
    // Process right child first 
    print2DUtil(root->right, space); 
  
    // Print current node after space 
    // count 
    printf("\n"); 
    for (int i = COUNT; i < space; i++) 
        printf(" "); 
    //printf("%ld\n", root->data.addr); 
    printf("%d\n", root->free);
    // Process left child 
    print2DUtil(root->left, space); 
} 
  
// Wrapper over print2DUtil() 
void print2D(tree* maple) 
{ 
   // Pass initial space count as 0 
   print2DUtil(maple->root, 0); 
} 

// searches for given value 
// if found returns the node (used for delete) 
// else returns the last node while traversing (used in insert) 
Node *search(tree* maple, tuple n) { 
	Node *temp = maple->root; 
	while (temp != NULL) { 
	if (n.addr < temp->data.addr) { 
		if (temp->left == NULL) 
		break; 
		else
		temp = temp->left; 
	} else if (n.addr == temp->data.addr) { 
		break; 
	} else { 
		if (temp->right == NULL) 
		break; 
		else
		temp = temp->right; 
	} 
	} 

	return temp; 
} 
/*
int main()
{
	tree *maple = malloc(sizeof(tree));
	maple->root = NULL;
	struct tuple t[30];
	int addrcount = 0;
	int lencount = 0;
    for (int i = 0; i < 20; i++) {
		t[i].addr = addrcount;
		t[i].len = lencount;
        insert(maple, t[i]);
		addrcount++;
		lencount++;
    }
	print2D(maple);
	deleteByVal(maple, t[0]);
	print2D(maple);
	deleteByVal(maple, t[1]);
	insert(maple, t[0]);
	print2D(maple);
}*/
