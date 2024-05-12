/*
 *  RAVL (augmented with Rank AVL) tree implementation.
 *  Author (starter code): Anya Tafliovich.
 *  Based on materials developed by F. Estrada.
 */

#include "RAVL_tree.h"

/*************************************************************************
 ** Suggested helper functions
 *************************************************************************/

/* Returns the height (number of nodes on the longest root-to-leaf path) of
 * the tree rooted at node 'node'. Returns 0 if 'node' is NULL.  Note: this
 * should be an O(1) operation.
 */
int height(RAVL_Node *node) {

  if (node == NULL) {
    return 0;
  }
  return node->height;
}

/* Returns the size (number of nodes) of the tree rooted at node 'node'.
 * Returns 0 if 'node' is NULL.  Note: this should be an O(1) operation.
 */
int size(RAVL_Node *node) {
  if (node == NULL) {
    return 0;
  }
  return node->size;
}

/* Updates the height of the tree rooted at node 'node' based on the heights
 * of its children. Note: this should be an O(1) operation.
 */
void updateHeight(RAVL_Node *node) {

  if (node == NULL) {
    return;
  }

  int left_height = 0;
  int right_height = 0;

  if (node->right != NULL) {
    right_height = node->right->height;
  }
  if (node->left != NULL) {
    left_height = node->left->height;
  }

  // take the max of the two heights
  if (left_height > right_height) {
    node->height = left_height + 1;
  } else {
    node->height = right_height + 1;
  }
}

/* Updates the size of the tree rooted at node 'node' based on the sizes
 * of its children. Note: this should be an O(1) operation.
 */
void updateSize(RAVL_Node *node) {
  if (node == NULL) {
    return;
  }

  int left_size = 0;
  int right_size = 0;

  if (node->right != NULL) {
    right_size = node->right->size;
  }
  if (node->left != NULL) {
    left_size = node->left->size;
  }
  node->size = left_size + right_size + 1;
}

/* Returns the balance factor (height of left subtree - height of right
 * subtree) of node 'node'. Returns 0 if node is NULL.  Note: this should be
 * an O(1) operation.
 */
int balanceFactor(RAVL_Node *node) {
  if (node == NULL) {
    return 0;
  }
  int left_height, right_height;

  if (node->right == NULL) {
    right_height = 0;
  } else {
    right_height = height(node->right);
  }

  if (node->left == NULL) {
    left_height = 0;
  } else {
    left_height = height(node->left);
  }

  return (left_height - right_height);
}

/* Returns the result of performing the corresponding rotation in the RAVL
 * tree rooted at 'node'.
 */
// single rotations: right/clockwise
RAVL_Node *rightRotation(RAVL_Node *node) {
  // if left child is null
  if (node == NULL || node->left == NULL) {
    return node;
  }

  // save head node in var
  RAVL_Node *new_head = node->left;
  RAVL_Node *shift = new_head->right;

  new_head->right = node;
  node->left = shift;

  updateHeight(node);
  updateSize(node);
  updateHeight(new_head);
  updateSize(new_head);

  return new_head;
}

// single rotations: left/counter-clockwise
RAVL_Node *leftRotation(RAVL_Node *node) {

  if (node == NULL || node->right == NULL) {
    return node;
  }

  RAVL_Node *new_head = node->right;
  RAVL_Node *shift = new_head->left;

  new_head->left = node;
  node->right = shift;

  updateHeight(node);
  updateSize(node);
  updateHeight(new_head);
  updateSize(new_head);

  return new_head;
}
// double rotation: right/clockwise then left/counter-clockwise
RAVL_Node *rightLeftRotation(RAVL_Node *node) {
  if (node == NULL || node->right == NULL) {
    return node;
  }

  node->right = rightRotation(node->right);

  return leftRotation(node);
}
// double rotation: left/counter-clockwise then right/clockwise
RAVL_Node *leftRightRotation(RAVL_Node *node) {
  if (node == NULL || node->left == NULL) {
    return node;
  }

  node->left = leftRotation(node->left);

  return rightRotation(node);
}

/* Returns the successor node of 'node'. */
RAVL_Node *successor(RAVL_Node *node) {
  RAVL_Node *successor = NULL; // Declare successor at the function scope

  if (node == NULL) {
    return NULL;
  }

  if (node->right != NULL) {
    successor = node->right;
    while (successor->left != NULL) {
      successor = successor->left;
    }
  }

  return successor;
}

/* Creates and returns an RAVL tree node with key 'key', value 'value', height
 * and size of 1, and left and right subtrees NULL.
 */
RAVL_Node *createNode(int key, void *value) {
  RAVL_Node *new_node = (RAVL_Node *)malloc(sizeof(RAVL_Node));
  if (new_node == NULL) {
    return NULL;
  }

  new_node->key = key;
  new_node->value = value;
  new_node->height = 1;
  new_node->size = 1;
  new_node->left = NULL;
  new_node->right = NULL;

  return new_node;
}

/*************************************************************************
 ** Provided functions
 *************************************************************************/

void printTreeInorder_(RAVL_Node *node, int offset) {
  if (node == NULL)
    return;
  printTreeInorder_(node->right, offset + 1);
  printf("%*s %d [%d / %d]\n", offset, "", node->key, node->height, node->size);
  printTreeInorder_(node->left, offset + 1);
}

void printTreeInorder(RAVL_Node *node) { printTreeInorder_(node, 0); }

void deleteTree(RAVL_Node *node) {
  if (node == NULL)
    return;
  deleteTree(node->left);
  deleteTree(node->right);
  free(node);
}

/*************************************************************************
 ** Required functions
 ** Must run in O(log n) where n is the number of nodes in a tree rooted
 **  at 'node'.
 *************************************************************************/

RAVL_Node *search(RAVL_Node *node, int key) {
  // base case
  if (node == NULL || node->key == key) {
    return node;
  }
  if (node->key < key) {
    return search(node->right, key);
  } else {
    return search(node->left, key);
  }
}

RAVL_Node *insert(RAVL_Node *node, int key, void *value) {

  if (node == NULL) {
    return createNode(key, value);
  }

  if (key < node->key) {
    node->left = insert(node->left, key, value);
  } else if (key > node->key) {
    node->right = insert(node->right, key, value);
  } else {
    return node;
  }
  updateHeight(node);
  updateSize(node);

  // balance
  int balance = balanceFactor(node);

  // left heavy
  if (balance > 1) {
    // left right case
    if (key > node->left->key) {
      return leftRightRotation(node);
    } else {
      return rightRotation(node);
    }
  }
  // right heavy
  if (balance < -1) {
    // right left case
    if (key < node->right->key) {
      return rightLeftRotation(node);
    } else {
      return leftRotation(node);
    }
  }
  return node;
}

RAVL_Node *delete(RAVL_Node *node, int key) {
  if (node == NULL) {
    return node;
  }

  if (key < node->key) {
    node->left = delete (node->left, key);
  } else if (key > node->key) {
    node->right = delete (node->right, key);
  } else {
    if (node->left == NULL || node->right == NULL) {
      RAVL_Node *temp = NULL;
      if (node->left == NULL && node->right == NULL) {
        temp = NULL;
      } else if (node->left == NULL) {
        temp = node->right;
      } else {
        temp = node->left;
      }
      if (temp == NULL) { // No children
        free(node);
        node = NULL;
      } else { // One child
        RAVL_Node *toFree = node;
        node = temp; // Directly use the child as the new node
        free(toFree);
      }
    } else {
      RAVL_Node *temp = successor(node);
      node->key = temp->key;
      node->right = delete (node->right, temp->key);
    }
  }
  updateHeight(node);
  updateSize(node);

  int balance = balanceFactor(node);

  // left heavy
  if (balance > 1 && balanceFactor(node->left) >= 0) {
    // Right rotation
    return rightRotation(node);
  } else if (balance > 1 && balanceFactor(node->left) < 0) {
    // Left-Right rotation
    return leftRightRotation(node);
  }

  if (balance < -1 && balanceFactor(node->right) >= 0) {
    // Right rotation
    return leftRotation(node);
  } else if (balance < -1 && balanceFactor(node->right) < 0) {
    // Left-Right rotation
    return rightLeftRotation(node);
  }
  return node;
}

int rank(RAVL_Node *node, int key) {
  if (node == NULL) {
    return NOTIN;
  }
  if (node->key == key) {
    return size(node->left) + 1;
  } else if (key > node->key) {
    return size(node->left) + 1 + rank(node->right, key);
  } else {
    return rank(node->left, key);
  }
}

RAVL_Node *findRank(RAVL_Node *node, int rank) {
  // deal with special case
  if (node == NULL) {
    return NULL;
  }
  int r = size(node->left) + 1;

  if (r == rank) {
    return node;
  } else if (rank < r) {
    return findRank(node->left, rank);
  } else {
    return findRank(node->right, rank - r);
  }
}
