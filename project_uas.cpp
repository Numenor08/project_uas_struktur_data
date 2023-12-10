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

class NodeAVL
{
public:
    AnggotaPerpustakaan *anggota;
    NodeAVL *left;
    NodeAVL *right;
    int height;
    NodeAVL(AnggotaPerpustakaan *a)
        : anggota(a), left(nullptr), right(nullptr), height(1) {}

    static int getHeight(NodeAVL *node)
    {
        if (node == nullptr)
            return 0;
        return node->height;
    }
    static int max(int a, int b)
    {
        return (a > b) ? a : b;
    }
    static NodeAVL *rotasiKanan(NodeAVL *y)
    {
        NodeAVL *x = y->left;
        NodeAVL *T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

        return x;
    }
    static NodeAVL *rotasiKiri(NodeAVL *x)
    {
        NodeAVL *y = x->right;
        NodeAVL *T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

        return y;
    }
    static int balancing(NodeAVL *node)
    {
        if (node == nullptr)
            return 0;
        return getHeight(node->left) - getHeight(node->right);
    }
    static NodeAVL *insert(NodeAVL *root, AnggotaPerpustakaan *anggota)
    {
        if (root == nullptr)
            return new NodeAVL(anggota);

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
    static NodeAVL *search(NodeAVL *root, int id)
    {
        if (root == nullptr || root->anggota->id == id)
            return root;

        if (id < root->anggota->id)
            return search(root->left, id);
        else if (id > root->anggota->id)
            return search(root->right, id);

        return nullptr;
    }
    static NodeAVL *minValueNode(NodeAVL *node)
    {
        NodeAVL *current = node;
        while (current->left != nullptr)
            current = current->left;

        return current;
    }
    static NodeAVL *deleteNode(NodeAVL *root, int id)
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
                NodeAVL *temp = root->right;
                delete root;
                return temp;
            }
            else if (root->right == nullptr)
            {
                NodeAVL *temp = root->left;
                delete root;
                return temp;
            }

            NodeAVL *temp = minValueNode(root->right);

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
    static void print(NodeAVL *root)
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
    static void saveToFile2(const string &filename, NodeAVL *rootAVL, ofstream &file)
    {
        if (rootAVL != nullptr)
        {
            file << rootAVL->anggota->id << "," << rootAVL->anggota->nama << "," << rootAVL->anggota->umur << "," << rootAVL->anggota->buku << "\n";
            if (rootAVL->left != nullptr)
                saveToFile2(filename, rootAVL->left, file);
            if (rootAVL->right != nullptr)
                saveToFile2(filename, rootAVL->right, file);
        }
    }
    static void saveToFile(const string &filename, NodeAVL *rootAVL)
    {
        ofstream file;
        file.open(filename, ios::out);
        saveToFile2(filename, rootAVL, file);
        file.close();
    }
    static NodeAVL *readFromFile(const string &filename)
    {
        ifstream file;
        file.open(filename, ios::in);
        if (!file.is_open())
        {
            return nullptr;
        }
        NodeAVL *root = nullptr;
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
            root = NodeAVL::insert(root, anggota);
        }
        file.close();
        return root;
    }
};

class NodeBst
{
public:
    AnggotaPerpustakaan *anggota;
    NodeBst *left;
    NodeBst *right;
    NodeBst(AnggotaPerpustakaan *a)
        : anggota(a), left(nullptr), right(nullptr) {}
    static NodeBst *insert(NodeBst *root, AnggotaPerpustakaan *anggota)
    {
        if (root == nullptr)
            return new NodeBst(anggota);

        if (anggota->id < root->anggota->id)
            root->left = insert(root->left, anggota);
        else if (anggota->id > root->anggota->id)
            root->right = insert(root->right, anggota);
        else {
            return root;
        }
        return root;
    }
    static NodeBst *search(NodeBst *root, int id)
    {
        if (root == nullptr || root->anggota->id == id)
            return root;

        if (id < root->anggota->id)
            return search(root->left, id);
        else if (id > root->anggota->id)
            return search(root->right, id);

        return nullptr;
    }
    static NodeBst *minValueNode(NodeBst *node)
    {
        NodeBst *current = node;
        while (current->left != nullptr)
            current = current->left;

        return current;
    }
    static NodeBst *deleteNode(NodeBst *root, int id)
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
                NodeBst *temp = root->right;
                delete root;
                return temp;
            }
            else if (root->right == nullptr)
            {
                NodeBst *temp = root->left;
                delete root;
                return temp;
            }

            NodeBst *temp = minValueNode(root->right);

            root->anggota->id = temp->anggota->id;
            root->anggota->nama = temp->anggota->nama;
            root->anggota->umur = temp->anggota->umur;
            root->anggota->buku = temp->anggota->buku;

            root->right = deleteNode(root->right, temp->anggota->id);
        }
        return root;
    }
    static void print(NodeBst *root)
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
    static void saveToFile2(const string &filename, NodeBst *&rootBST, ofstream &file)
    {
        if (rootBST != nullptr)
        {
            file << rootBST->anggota->id << "," << rootBST->anggota->nama << "," << rootBST->anggota->umur << "," << rootBST->anggota->buku << "\n";
            if (rootBST->left != nullptr)
                NodeBst::saveToFile2(filename, rootBST->left, file);
            if (rootBST->right != nullptr)
                NodeBst::saveToFile2(filename, rootBST->right, file);
        }
    }
    static void saveToFile(const string &filename, NodeBst *&rootBST)
    {
        ofstream file;
        file.open(filename, ios::out);
        NodeBst::saveToFile2(filename, rootBST, file);
        file.close();
    }
    static NodeBst *readFromFile(const string &filename)
    {
        ifstream file;
        file.open(filename, ios::in);
        if (!file.is_open())
        {
            return nullptr;
        }
        NodeBst *root = nullptr;
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
            root = NodeBst::insert(root, anggota);
        }
        file.close();
        return root;
    }
};

int validationInt(const string &text, int max = INT_MAX)
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
    while (angka > max)
    {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Input melebihi batas!\n";
        cout << text; cin >> angka;
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
    NodeAVL *rootAVL = nullptr;
    NodeBst *rootBST = nullptr;
    NodeAVL *result = nullptr;
    NodeBst *result2 = nullptr;
    bool loop1 = true;
    int pilihan, id, umur;
    string buku, nama;
    cout << "Pilih struktur!\n";
    cout << "[1] BST\n";
    cout << "[2] AVL\n";
    pilihan = validationInt("Pilihan: ", 2);
    switch (pilihan)
    {
    case 1:
        system("cls");
        rootBST = NodeBst::readFromFile("data_anggota.txt");
        cout << "-----------------------------------------------\n";
        cout << "PROGRAM ADMINISTRASI ANGGOTA PERPUSTAKAAN (BST)\n";
        cout << "-----------------------------------------------\n";
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
                rootBST = NodeBst::insert(rootBST, new AnggotaPerpustakaan(id, nama, umur, buku));
                NodeBst::saveToFile("data_anggota.txt", rootBST);
                system("cls");
                cout << "Data berhasil masuk!\n";
                break;
            case 2:
                id = validationInt("Masukkan ID yang akan dihapus: ");
                result2 = NodeBst::search(rootBST, id);
                if (result2 != nullptr)
                {
                    rootBST = NodeBst::deleteNode(rootBST, id);
                    NodeBst::saveToFile("data_anggota.txt", rootBST);
                    system("cls");
                    cout << "Anggota dengan ID " << id << " berhasil dihapus!\n";
                }
                else
                {
                    cout << "Anggota dengan ID " << id << " tidak ada!\n";
                }
                break;
            case 3:
                id = validationInt("Masukkan ID yang dicari: ");
                result2 = NodeBst::search(rootBST, id);
                system("cls");
                if (result2 != nullptr)
                {
                    cout << "Anggota ditemukan!\n";
                    cout << "ID: " << result2->anggota->id << endl;
                    cout << "Nama: " << result2->anggota->nama << endl;
                    cout << "Umur: " << result2->anggota->umur << endl;
                    cout << "Buku: " << result2->anggota->buku << endl;
                }
                else
                {
                    cout << "Anggota dengan ID " << id << " tidak ditemukan\n";
                }
                break;
            case 4:
                system("cls");
                cout << "Daftar Anggota Perpustakaan: \n";
                NodeBst::print(rootBST);
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
        delete rootBST;
        break;
    case 2:
        system("cls");
        rootAVL = NodeAVL::readFromFile("data_anggota.txt");
        cout << "-----------------------------------------------\n";
        cout << "PROGRAM ADMINISTRASI ANGGOTA PERPUSTAKAAN (BST)\n";
        cout << "-----------------------------------------------\n";
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
                rootAVL = NodeAVL::insert(rootAVL, new AnggotaPerpustakaan(id, nama, umur, buku));
                NodeAVL::saveToFile("data_anggota.txt", rootAVL);
                system("cls");
                cout << "Data berhasil masuk!\n";
                break;
            case 2:
                id = validationInt("Masukkan ID yang akan dihapus: ");
                result = NodeAVL::search(rootAVL, id);
                if (result != nullptr)
                {
                    rootAVL = NodeAVL::deleteNode(rootAVL, id);
                    NodeAVL::saveToFile("data_anggota.txt", rootAVL);
                    system("cls");
                    cout << "Anggota dengan ID " << id << " berhasil dihapus!\n";
                }
                else
                {
                    cout << "Anggota dengan ID " << id << " tidak ada!\n";
                }
                break;
            case 3:
                id = validationInt("Masukkan ID yang dicari: ");
                result = NodeAVL::search(rootAVL, id);
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
                NodeAVL::print(rootAVL);
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
        break;

    default:
        break;
    }
    return 0;
}