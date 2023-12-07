#include <fstream>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

class AnggotaPerpustakaan
{
public:
    int id;
    string nama;
    int umur;
    string buku;
    AnggotaPerpustakaan(int i, const string &n, int u, const string &b)
        : id(i), nama(n), umur(u), buku(b) {}
};

class Node
{
public:
    AnggotaPerpustakaan *anggota;
    Node *left;
    Node *right;
    int height;
    Node(AnggotaPerpustakaan *a)
        : anggota(a), left(nullptr), right(nullptr), height(1) {}

    static int getHeight(Node *node)
    {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    static int max(int a, int b)
    {
        return (a > b) ? a : b;
    }

    static Node *rotasiKanan(Node *y)
    {
        Node *x = y->left;
        Node *T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

        return x;
    }

    static Node *rotasiKiri(Node *x)
    {
        Node *y = x->right;
        Node *T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

        return y;
    }

    static int balancing(Node *node)
    {
        if (node == nullptr)
            return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

    static Node *insert(Node *root, AnggotaPerpustakaan *anggota)
    {
        if (root == nullptr)
            return new Node(anggota);

        if (anggota->id < root->anggota->id)
            root->left = insert(root->left, anggota);
        else if (anggota->id > root->anggota->id)
            root->right = insert(root->right, anggota);
        else
            return root;

        root->height = 1 + max(getHeight(root->left), getHeight(root->right));

        int balance = balancing(root);

        if (balance > 1 && anggota->id < root->left->anggota->id)
            return rotasiKanan(root);

        if (balance < -1 && anggota->id > root->right->anggota->id)
            return rotasiKiri(root);

        if (balance > 1 && anggota->id > root->left->anggota->id)
        {
            root->left = rotasiKiri(root->left);
            return rotasiKanan(root);
        }

        if (balance < -1 && anggota->id < root->right->anggota->id)
        {
            root->right = rotasiKanan(root->right);
            return rotasiKiri(root);
        }

        return root;
    }

    static Node *search(Node *root, int id)
    {
        if (root == nullptr || root->anggota->id == id)
            return root;

        if (id < root->anggota->id)
            return search(root->left, id);
        else if (id > root->anggota->id)
            return search(root->right, id);

        return nullptr;
    }

    static Node *minValueNode(Node *node)
    {
        Node *current = node;
        while (current->left != nullptr)
            current = current->left;

        return current;
    }

    static Node *deleteNode(Node *root, int id)
    {
        if (root == nullptr)
            return root;
        if (id < root->anggota->id)
            root->left = deleteNode(root->left, id);
        else if (id > root->anggota->id)
            root->right = deleteNode(root->right, id);
        else
        {
            if (root->left == nullptr)
            {
                Node *temp = root->right;
                delete root;
                return temp;
            }
            else if (root->right == nullptr)
            {
                Node *temp = root->left;
                delete root;
                return temp;
            }

            Node *temp = minValueNode(root->right);

            root->anggota->id = temp->anggota->id;
            root->anggota->nama = temp->anggota->nama;
            root->anggota->umur = temp->anggota->umur;
            root->anggota->buku = temp->anggota->buku;

            root->right = deleteNode(root->right, temp->anggota->id);
        }

        if (root == nullptr)
            return root;

        root->height = 1 + max(getHeight(root->left), getHeight(root->right));
        int balance = balancing(root);
        if (balance > 1 && balancing(root->left) >= 0)
            return rotasiKanan(root);

        if (balance > 1 && balancing(root->left) < 0)
        {
            root->left = rotasiKiri(root->left);
            return rotasiKanan(root);
        }

        if (balance < -1 && balancing(root->right) <= 0)
            return rotasiKiri(root);

        if (balance < -1 && balancing(root->right) > 0)
        {
            root->right = rotasiKanan(root->right);
            return rotasiKiri(root);
        }
        return root;
    }

    static void print(Node *root)
    {
        if (root != nullptr)
        {
            print(root->left);
            cout << "ID: " << root->anggota->id << endl;
            cout << "Nama: " << root->anggota->nama << endl;
            cout << "Umur: " << root->anggota->umur << endl;
            cout << "Buku yang dipinjam: " << root->anggota->buku << endl
                 << endl;
            print(root->right);
        }
    }
};

void saveToFile(const string &filename, Node *rootAVL, ofstream &file)
{
    if (rootAVL != nullptr)
    {
        file << rootAVL->anggota->id << "," << rootAVL->anggota->nama << "," << rootAVL->anggota->umur << "," << rootAVL->anggota->buku << "\n";
        if (rootAVL->left != nullptr)
            saveToFile(filename, rootAVL->left, file);
        if (rootAVL->right != nullptr)
            saveToFile(filename, rootAVL->right, file);
    }
}
void saveToFile(const string &filename, Node *rootAVL)
{
    ofstream file;
    file.open(filename, ios::out);
    saveToFile(filename, rootAVL, file);
    file.close();
}

Node *readFromFile(const string &filename)
{
    ifstream file;
    file.open(filename, ios::in);
    if (!file.is_open())
    {
        return nullptr;
    }
    Node *root = nullptr;
    while (!file.eof())
    {
        int id, umur;
        string nama, buku;
        file >> id;
        file.ignore();
        getline(file, nama, ',');
        file >> umur;
        file.ignore();
        getline(file, buku);
        AnggotaPerpustakaan *anggota = new AnggotaPerpustakaan(id, nama, umur, buku);
        root = Node::insert(root, anggota);
    }
    file.close();
    return root;
}

int validationInt(const string &text)
{
    int angka;
    cout << text;
    while (!(cin >> angka))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input harus integer!\n";
        cout << text;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return angka;
}

string validationString(const string &text)
{
    string input;
    do
    {
        cout << text;
        getline(cin, input);
        if (input.empty())
        {
            cout << "Input tidak boleh kosong!\n";
        }

    } while (input.empty());
    return input;
}

int main(int argc, char const *argv[])
{
    Node *rootAVL = nullptr;
    Node *result = nullptr;
    bool loop1 = true;
    int pilihan, id, umur;
    string buku, nama;
    rootAVL = readFromFile("data_anggota.txt");
    cout << "-----------------------------------------\n";
    cout << "PROGRAM ADMINISTRASI ANGGOTA PERPUSTAKAAN\n";
    cout << "-----------------------------------------\n";
    while (loop1 == true)
    {
        cout << "\nMenu:\n";
        cout << "[1] Insert\n";
        cout << "[2] Delete\n";
        cout << "[3] Search\n";
        cout << "[4] View (ALL)\n";
        cout << "[5] Keluar\n";
        pilihan = validationInt("Pilihan: ");
        switch (pilihan)
        {
        case 1:
            id = validationInt("Masukkan ID: ");
            nama = validationString("Masukkan Nama: ");
            umur = validationInt("Masukkan Umur: ");
            buku = validationString("Masukkan Buku yang dipinjam: ");
            rootAVL = Node::insert(rootAVL, new AnggotaPerpustakaan(id, nama, umur, buku));
            saveToFile("data_anggota.txt", rootAVL);
            system("cls");
            cout << "Data berhasil masuk!\n";
            break;
        case 2:
            id = validationInt("Masukkan ID yang akan dihapus: ");
            result = Node::search(rootAVL, id);
            if (result != nullptr)
            {
                rootAVL = Node::deleteNode(rootAVL, id);
                saveToFile("data_anggota.txt", rootAVL);
                system("cls");
                cout << "Anggota dengan ID " << id << " berhasil dihapus!\n";
            } else {
                cout << "Anggota dengan ID " << id << " tidak ada!\n";
            }
            break;
        case 3:
            id = validationInt("Masukkan ID yang dicari: ");
            result = Node::search(rootAVL, id);
            system("cls");
            if (result != nullptr)
            {
                cout << "Anggota ditemukan!\n";
                cout << "ID: " << result->anggota->id << endl;
                cout << "Nama: " << result->anggota->nama << endl;
                cout << "Umur: " << result->anggota->umur << endl;
                cout << "Buku: " << result->anggota->buku << endl;
            }
            else
            {
                cout << "Anggota dengan ID " << id << " tidak ditemukan\n";
            }
            break;
        case 4:
            system("cls");
            cout << "Daftar Anggota Perpustakaan: \n";
            Node::print(rootAVL);
            break;
        case 5:
            loop1 = false;
            break;
        default:
            system("cls");
            cout << "Input Melebihi Batas!\n";
            break;
        }
    }
    delete rootAVL;
    cin.get();
    return 0;
}