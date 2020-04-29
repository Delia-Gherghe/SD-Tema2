#include <iostream>
#include <cmath>
#include <fstream>
using namespace std;

struct nod
{
    nod* parent;
    nod* child;
    nod* left;
    nod* right;
    ///bool del;
    int key;
    int deg;
};

void link(nod *&H, nod *&p1, nod *p2)       /// functie de lipire a lui p2 ca fiu al lui p1
{
    (p2 -> left) -> right = p2 -> right;
    (p2 -> right) -> left = p2 -> left;
    if (p1 -> right == p1)
        H = p1;
    p2 -> left = p2;
    p2 -> right = p2;
    p2 -> parent = p1;
    if (p1 -> child == NULL)
        p1 -> child = p2;
    p2 -> right = p1 -> child;
    p2 -> left = (p1 -> child) -> left;
    ((p1 -> child) -> left) -> right = p2;
    (p1 -> child) -> left = p2;
    if (p2 -> key < (p1 -> child) -> key)
        p1 -> child = p2;
    p1 -> deg ++;
}

void consolidate(nod *& H, int n)       /// functie ce consolideaza lista de radacini a heap-ului
{
    int x1;
    int x2 = (int)(log(n)/log(2));      /// calculam maximul de fii pe care ii poate avea un nod in heap-ul cu n noduri
    nod *arr[x2];
    for (int i = 0; i <= x2; i++)
        arr[i] = NULL;
    nod* p1 = H;
    nod* p2;
    nod* p3;
    do                                  /// parcurgem lista de radacini pt a putea consolida heap-urile de acelasi grad
    {                                   /// pana cand radacinile ramase au gradul distinct
        x1 = p1 -> deg;
        while (arr[x1] != NULL)
        {
            p2 = arr[x1];
            if (p1 -> key > p2 -> key)  /// daca gasim doua radacini cu acelasi grad le interschimbam astfel incat p1
            {                           /// sa pointeze catre radacina cu cheia mai mica
                p3 = p1;
                p1 = p2;
                p2 = p3;
            }
            if (p2 == H)
                H = p1;
            link(H, p1, p2);            /// legam p2 de p1
            if (p1 -> right == p1)
                H = p1;
            arr[x1] = NULL;
            x1 ++;
        }
        arr[x1] = p1;
        p1 = p1 -> right;
    } while (p1 != H);

    p1 = H;
    p2 = H -> right;
    while (p2 != p1)                    /// actualizam minimul
    {
        if (p2 -> key < H -> key)
            H = p2;
        p2 = p2 -> right;
    }
}

void delete_min(nod *&H, int &n)        /// functie pentru stergerea minimului din heap
{
    if (H == NULL)
        cout << "The heap is empty" << endl;
    else
    {
        nod* t = H;
        nod* p;
        p = t;
        nod* x = NULL;
        if (t -> child != NULL)
        {
            x = t -> child;
            do                          /// adaugam toti fiii minimului in lista de radacini
            {
                p = x -> right;
                (H -> left) -> right = x;
                x -> right = H;
                H -> left = x;
                if (x -> key < H -> key)
                    H = x;
                x -> parent = NULL;
                x = p;
            } while (p != t -> child);
        }
        (t -> left) -> right = t -> right;      /// eliminam minimul
        (t -> right) -> left = t -> left;
        H = t -> right;
        if (t == t -> right && t -> child == NULL)
            H = NULL;
        else
        {
            H = t -> right;
            consolidate(H,n);                   /// consolidam lista de radacini
        }
        n--;
    }
}

void insertion(nod *&H, int val, int &n)        /// functie pentru inserarea unui nod in heap
{
    nod* new_nod = new nod;
    new_nod -> key = val;                       /// creem noul nod
    new_nod -> parent = NULL;
    new_nod -> child = NULL;
    new_nod -> left = new_nod;
    new_nod -> right = new_nod;
    ///new_nod -> del = false;
    new_nod -> deg = 0;
    if (H != NULL)
    {
        (H -> left) -> right = new_nod;         /// adaugam nodul in lista de radacin
        new_nod -> right = H;
        new_nod -> left = H -> left;
        H -> left = new_nod;
        if (new_nod -> key < H -> key)          /// daca este cazul, actualizam minimul
            H = new_nod;

    }
    else
        H = new_nod;
    n++;
}

void afis(nod* H, int n)    /// functie de afisare a listei de radacini si a numarului nodurilor
{
    nod* p = H;
    if (p == NULL)
        cout << "The heap is empty!" << endl;
    else
    {
        cout << "The root nodes of the heap are:" <<endl;
        do
        {
            cout << p -> key;
            p = p -> right;
            if (p != H)
                cout << "-->";
        } while (p -> right != NULL && p != H);

        cout << endl << "The heap has " << n << " nodes!" <<endl;
    }
}

void find_minim(nod* H)         /// functie de afisare a minimului
{
    cout << "Min of heap is " << H -> key << endl;
}

void uni(nod *&H, nod *J)       /// functie de merge ( alipim heap-ul J la heap-ul H )
{
    if (H == NULL)
        H = J;
    else
        if (H != NULL && J != NULL)
    {
        nod* Hr = H -> right;
        nod* Jl = J -> left;
        H -> right = J;
        J -> left = H;
        Hr -> left = Jl;
        Jl -> right = Hr;

        if (J -> key < H -> key)
            H = J;

    }
}

/**
void erase_val(nod *&H, int val)
{
    nod* current = H;
    bool found = false;
    if  (current!= NULL)
    do
    {
        if (current -> key == val)
            {current -> del = true;
             found = true;}
        else
        {
            if(val > current -> key)
                erase_val(current->child,val);
            current = current -> right;
        }
    }while(current!=H && found == false);

}
**/

void lazy_deletion(nod* H, int del[], int &ndel, int val)
{
    del[ndel] = val;
    ndel++;
}

void build(nod *&H, int &nh, int v[], int nv)
{
    for (int i = 0; i < nv; i++)
        insertion(H, v[i], nh);
}

int main()
{
    ifstream fin("Fibonacci.in");
    nod* H = NULL;
    int nh = 0;
    int n, x, y, ndelH = 0;
    int delH[100];
    fin >> n;

    for (int i = 0; i < n; i++)
    {
        fin >> x;
        if (x == 1)
        {
            fin >> y;
            insertion(H, y, nh);
        }
        else
            if (x == 2)
        {
            fin >> y;
            lazy_deletion(H, delH, ndelH, y);
        }
        else if (x == 3)
            find_minim(H);

        else if (x == 4)
            delete_min(H, nh);
    }

    for (int j = 0; j< ndelH; j++)
        cout<<delH[j]<<" ";
    cout << endl;

    afis(H, nh);

    fin.close();

    return 0;
}
