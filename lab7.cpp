#include <iostream>
using namespace std;
typedef enum { BLACK, RED } nodeColor;
typedef int Elem;
Elem MAX(Elem a, Elem b)
{
    return a > b ? a : b;
}
typedef struct RBNode {
    Elem elem;     // полезные данные
    struct RBNode* lc;  // ссылки на левого 
    struct RBNode* rc;  // и правого сыновей
    struct RBNode* parent; // ссылка на родителя
    nodeColor color;   // цвет узла
}Node;
#define NIL &sentinel           /* all leafs are sentinels */
Node sentinel = { 0,NIL, NIL, NIL,BLACK };
Node* ROOT = NIL;
int GetHeight(RBNode* node)
{
    if (node == NULL)
    {
        return 0;
    }
    else
    {
        return 1 + MAX(GetHeight(node->lc), GetHeight(node->rc));
    }
 
}
RBNode* Search(RBNode* root, int el)
{
    while (root->elem != el)
    {
        if (root == NULL) return root;
        if (root->elem > el) root = root->lc;
        if (root->elem < el) root = root->rc;
    }
    return root;
}
void Leftrotate(Node* x) {
 
    Node* y = x->rc;
 
    x->rc = y->lc;
    if (y->lc != NIL) y->lc->parent = x;
    y->parent = x->parent;
    if (x->parent == NIL) ROOT = y;
    else if (x == x->parent->lc)
        x->parent->lc = y;
    else
        x->parent->rc = y;
    y->lc = x;
    x->parent = y;
}
void Rightrotate(Node* x) {
 
    Node* y = x->lc;
    x->lc = y->rc;
    if (y->rc != NIL) y->rc->parent = x;
    y->parent = x->parent;
    if (x->parent == NIL) ROOT = y;
    else  if (x == x->parent->lc)
        x->parent->lc = y;
    else
        x->parent->rc = y;
    y->rc = x;
    x->parent = y;
}
void InsertFixup(Node* z)
{
    while (z->parent->color == RED)
    {
        if (z->parent == z->parent->parent->lc)
        {
            Node* y = z->parent->parent->rc;
            if (y->color == RED)
            {/* uncle is RED */
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {/* uncle is BLACK */
                if (z == z->parent->rc)
                {/* make x a left child */
                    z = z->parent;
                    Leftrotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                Rightrotate(z->parent->parent);
            }
        }
        else
        {
            Node* y = z->parent->parent->lc;
            if (y->color == RED)
            {/* uncle is RED */
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {/* uncle is BLACK */
                if (z == z->parent->lc)
                {
                    z = z->parent;
                    Rightrotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                Leftrotate(z->parent->parent);
            }
        }
    }
    ROOT->color = BLACK;
}
RBNode* insert(Elem e)
{
    Node* y = NIL;
    Node* x = ROOT;
    Node* z = new Node{e,NIL,NIL,NIL,RED};
    while (x != NIL)
    {
        y = x;
        if (z->elem < x->elem) x = x->lc;
        else x = x->rc;
    }
    z->parent = y;
    if (y == NIL) ROOT = z;
    else if (z->elem < y->elem)
        y->lc = z;
    else y->rc = z;
  //  if (z != ROOT) z->color = RED;
    InsertFixup(z);
    return z;
}
void Transplant(Node* u, Node* v)
{
    if (u->parent == NIL) ROOT = v;
    else if (u == u->parent->lc) u->parent->lc = v;
    else u->parent->rc = v;
    v->parent = u->parent;
}
Node* TreeMinimum(Node* x)
{
    while (x->lc != NIL)
    {
        x = x->lc;
    }
    return x;
}
void DeleteFixup(Node* x)
{
    while (x != ROOT && x->color == BLACK)
    {
        if (x == x->parent->lc)
        {
            Node* w = x->parent->rc;
            if (w->color == RED)
            {
                w->color = BLACK;
                w->parent->color = RED;
                Leftrotate(x->parent);
                w = x->parent->rc;
            }
            if (w->lc->color == BLACK && w->rc->color == BLACK)
            {
                w->color = RED;
                x = x->parent;
            }
            else if (w->rc->color == BLACK)
            {
                w->lc->color = BLACK;
                w->color = RED;
                Rightrotate(w);
                w = x->parent->rc;
            }
            w->color = x->parent->color;
            x->parent->color = BLACK;
            w->rc->color = BLACK;
            Leftrotate(x->parent);
            x = ROOT;
        }
        else
        {
            Node* w = x->parent->lc;
            if (w->color == RED)
            {
                w->color = BLACK;
                w->parent->color = RED;
                Rightrotate(x->parent);
                w = x->parent->lc;
            }
            if (w->rc->color == BLACK && w->lc->color == BLACK)
            {
                w->color = RED;
                x = x->parent;
            }
            else if (w->lc->color == BLACK)
            {
                w->rc->color = BLACK;
                w->color = RED;
                Leftrotate(w);
                w = x->parent->lc;
            }
            w->color = x->parent->color;
            x->parent->color = BLACK;
            w->lc->color = BLACK;
            Rightrotate(x->parent);
            x = ROOT;
        }
    }
    x->color = BLACK;
}
void Delete(Node* z)
{
    Node* y = z;
    Node* x = NIL;
    nodeColor y_orig = y->color;
    if (z->lc == NIL)
    {
        x = z->rc;
        Transplant(z, z->rc);
    }
    else if (z->rc == NIL)
    {
        x = z->lc;
        Transplant(z, z->lc);
    }
    else
    {
        y = TreeMinimum(z->rc);
        nodeColor y_orig = y->color;
        x = z->rc;
        if (y->parent == z) x->parent = y;
        else
        {
            Transplant(y, y->rc);
            y->rc = z->rc;
            y->rc->parent = y;
        }
        Transplant(z, y);
        y->lc = z->lc;
        y->lc->parent = y;
        y->color = z->color;
    }
    if (y_orig == BLACK) DeleteFixup(x);
}
void Print(Node* p, int level = 0)
{
    if (p != NIL)
    {
        Print(p->lc, level + 1);
        for (int i = 0; i < level; i++) cout << "   ";
        cout << p->elem << " " << p->color << endl;
        Print(p->rc, level + 1);
    }
}
int main()
{
    insert(100);
    insert(90);
    insert(80);
    insert(70);
    insert(60);
    insert(19);
    insert(103);
    Node* z = insert(109);
    insert(119);
    Print(ROOT);
    Delete(z);
    Print(ROOT);
}
