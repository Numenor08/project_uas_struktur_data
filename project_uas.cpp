#include <cstdio>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <sys/stat.h>
#include <vector>

using namespace std;

int validationInt(const string &text, int max = INT_MAX);
string validationString(const string &text);

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
    static int max(int a, int b) { return (a > b) ? a : b; }
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
    static void saveToFile2(const string &fileDir, NodeAVL *rootAVL,
                            ofstream &file)
    {
        if (rootAVL != nullptr)
        {
            file << rootAVL->anggota->id << "," << rootAVL->anggota->nama << ","
                 << rootAVL->anggota->umur << "," << rootAVL->anggota->buku << "\n";
            if (rootAVL->left != nullptr)
                saveToFile2(fileDir, rootAVL->left, file);
            if (rootAVL->right != nullptr)
                saveToFile2(fileDir, rootAVL->right, file);
        }
    }
    static void saveToFile(const string &fileName, NodeAVL *rootAVL)
    {
        ofstream file;
        string fileDir = "./src/" + fileName + ".txt";
        file.open(fileDir, ios::out);
        saveToFile2(fileDir, rootAVL, file);
        file.close();
    }
    static NodeAVL *readFromFile(const string &fileName)
    {
        ifstream file;
        string fileDir = "./src/" + fileName + ".txt";
        file.open(fileDir, ios::in);
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
            AnggotaPerpustakaan *anggota =
                new AnggotaPerpustakaan(id, nama, umur, buku);
            root = NodeAVL::insert(root, anggota);
        }
        file.close();
        return root;
    }
    static void clearHeap(NodeAVL *root)
    {
        if (root == nullptr)
        {
            return;
        }
        clearHeap(root->left);
        clearHeap(root->right);
        delete root->anggota;
        delete root;
    }
    static void menu(const string &fileName)
    {
        NodeAVL *rootAVL = nullptr;
        NodeAVL *result = nullptr;
        bool loop1 = true;
        int pilihan, id, umur;
        string buku, nama;
        system("cls");
        rootAVL = NodeAVL::readFromFile(fileName);
        cout << "-----------------------------------------------\n";
        cout << "PROGRAM ADMINISTRASI ANGGOTA PERPUSTAKAAN (AVL)\n";
        cout << "-----------------------------------------------\n";
        cout << "Nama perpustakaan: " << fileName << endl;
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
                rootAVL =
                    insert(rootAVL, new AnggotaPerpustakaan(id, nama, umur, buku));
                saveToFile(fileName, rootAVL);
                system("cls");
                cout << "Data berhasil masuk!\n";
                break;
            case 2:
                id = validationInt("Masukkan ID yang akan dihapus: ");
                result = search(rootAVL, id);
                if (result != nullptr)
                {
                    rootAVL = deleteNode(rootAVL, id);
                    saveToFile(fileName, rootAVL);
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
                result = search(rootAVL, id);
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
                print(rootAVL);
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
        clearHeap(rootAVL);
    }
};
class NodeBst
{
public:
    AnggotaPerpustakaan *anggota;
    NodeBst *left;
    NodeBst *right;
    NodeBst(AnggotaPerpustakaan *a) : anggota(a), left(nullptr), right(nullptr) {}
    static NodeBst *insert(NodeBst *root, AnggotaPerpustakaan *anggota)
    {
        if (root == nullptr)
            return new NodeBst(anggota);

        if (anggota->id < root->anggota->id)
            root->left = insert(root->left, anggota);
        else if (anggota->id > root->anggota->id)
            root->right = insert(root->right, anggota);
        else
        {
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
    static void saveToFile2(const string &fileDir, NodeBst *&rootBST,
                            ofstream &file)
    {
        if (rootBST != nullptr)
        {
            file << rootBST->anggota->id << "," << rootBST->anggota->nama << ","
                 << rootBST->anggota->umur << "," << rootBST->anggota->buku << "\n";
            if (rootBST->left != nullptr)
                NodeBst::saveToFile2(fileDir, rootBST->left, file);
            if (rootBST->right != nullptr)
                NodeBst::saveToFile2(fileDir, rootBST->right, file);
        }
    }
    static void saveToFile(const string &fileName, NodeBst *&rootBST)
    {
        ofstream file;
        string fileDir = "./src/" + fileName + ".txt";
        file.open(fileDir, ios::out);
        NodeBst::saveToFile2(fileDir, rootBST, file);
        file.close();
    }
    static NodeBst *readFromFile(const string &fileName)
    {
        ifstream file;
        string fileDir = "./src/" + fileName + ".txt";
        file.open(fileDir, ios::in);
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
            AnggotaPerpustakaan *anggota =
                new AnggotaPerpustakaan(id, nama, umur, buku);
            root = NodeBst::insert(root, anggota);
        }
        file.close();
        return root;
    }
    static void clearHeap(NodeBst *root)
    {
        if (root == nullptr)
        {
            return;
        }
        clearHeap(root->left);
        clearHeap(root->right);
        delete root->anggota;
        delete root;
    }
    static void menu(const string &fileName)
    {
        NodeBst *rootBST = nullptr;
        NodeBst *result = nullptr;
        bool loop1 = true;
        int pilihan, id, umur;
        string buku, nama;
        system("cls");
        rootBST = readFromFile(fileName);
        cout << "-----------------------------------------------\n";
        cout << "PROGRAM ADMINISTRASI ANGGOTA PERPUSTAKAAN (BST)\n";
        cout << "-----------------------------------------------\n";
        cout << "Nama perpustakaan: " << fileName << endl;
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
                rootBST =
                    insert(rootBST, new AnggotaPerpustakaan(id, nama, umur, buku));
                saveToFile(fileName, rootBST);
                system("cls");
                cout << "Data berhasil masuk!\n";
                break;
            case 2:
                id = validationInt("Masukkan ID yang akan dihapus: ");
                result = search(rootBST, id);
                if (result != nullptr)
                {
                    rootBST = deleteNode(rootBST, id);
                    saveToFile(fileName, rootBST);
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
                result = search(rootBST, id);
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
                print(rootBST);
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
        clearHeap(rootBST);
    }
};
struct Perpustakaan
{
    void savePerpus(vector<string> &perpus)
    {
        ofstream file;
        string folder = "./src";
        string fileDir = folder + "/daftar_perpustakaan" + ".txt";
        mkdir(folder.c_str());
        file.open(fileDir, ios::out);
        for (const auto &value : perpus)
        {
            file << value << endl;
        }
        file.close();
    }
    void readPerpus(vector<string> &perpus)
    {
        ifstream file;
        string fileDir = "./src/daftar_perpustakaan.txt";
        file.open(fileDir, ios::in);
        if (!file.is_open())
        {
            return;
        }
        string line;
        while (getline(file, line))
        {
            perpus.push_back(line);
        }
        file.close();
        createPerpus(perpus);
    }
    void printPerpus(vector<string> &perpus)
    {
        if (perpus.empty())
        {
            cout << "Kosong!\n";
        }
        else
        {
            int i = 1;
            for (const auto &value : perpus)
            {
                cout << "[" << i << "] " << value << endl;
                i++;
            }
        }
    }
    void deletePerpus(vector<string> &perpus, int &max)
    {
        int index;
        char pilihanChar;
        index = validationInt("Pilih yang akan dihapus (nomor): ", max);
        cout << "Apakah anda yakin ingin menghapus data tersebut? (y/n)\n";
        cin >> pilihanChar;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (pilihanChar == 'y' || pilihanChar == 'Y')
        {
            index = index - 1;
            string fileDir = "./src/" + perpus.at(index) + ".txt";
            remove(fileDir.c_str());
            perpus.erase(perpus.begin() + index);
            savePerpus(perpus);
            system("cls");
            cout << "Data berhasil dihapus!\n";
        }
        else
        {
            system("cls");
        }
    }
    void createPerpus(vector<string> &perpus)
    {
        for (const auto &value : perpus)
        {
            string folder = "./src";
            mkdir(folder.c_str());
            string fileDir = folder + "/" + value + ".txt";
            ofstream file;
            file.open(fileDir, ios::out | ios::app);
            file.close();
        }
    }
    void createPerpusBaru(vector<string> &perpus, const string &fileName)
    {
        perpus.push_back(fileName);
        savePerpus(perpus);
        createPerpus(perpus);
    }
};

int main(int argc, char const *argv[])
{
    int pilihan, pilihan2;
    vector<string> perpus;
    bool loop = true;
    Perpustakaan perpustakaan;

    cout << "Pilih struktur!\n";
    cout << "[1] BST\n";
    cout << "[2] AVL\n";
    pilihan = validationInt("Pilihan: ", 2);
    system("cls");
    perpustakaan.readPerpus(perpus);
    do
    {
        cout << "Daftar Perpustakaan: \n";
        int max = perpus.size();
        perpustakaan.printPerpus(perpus);
        cout << "\nMenu:\n";
        cout << "[a] Pilih perpustakaan\n";
        cout << "[b] Tambah perpustakaan\n";
        cout << "[c] Hapus perpustakaan\n";
        cout << "[d] Keluar\n";
        cout << "Pilihan (huruf): ";
        char pilihanChar;
        cin >> pilihanChar;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (pilihanChar == 'a' || pilihanChar == 'A')
        {
            pilihan2 = validationInt("Pilih perpustakaan (angka): ", max);
            loop = false;
        }
        else if (pilihanChar == 'b' || pilihanChar == 'B')
        {
            string nama = validationString("Masukkan nama perpustakaannya: ");
            system("cls");
            perpustakaan.createPerpusBaru(perpus, nama);
        }
        else if (pilihanChar == 'c' || pilihanChar == 'C')
        {
            if (!perpus.empty())
            {
                perpustakaan.deletePerpus(perpus, max);
            }
            else
            {
                system("cls");
                cout << "Tidak ada data yang bisa dihapus!\n";
            }
        }
        else if (pilihanChar == 'd' || pilihanChar == 'D')
        {
            return 0;
        }
        else
        {
            system("cls");
            cout << "Input tidak valid!\n";
        }
    } while (loop == true);

    string value = perpus.at(pilihan2 - 1);

    switch (pilihan)
    {
    case 1:
        NodeBst::menu(value);
        break;
    case 2:
        NodeAVL::menu(value);
        break;
    default:
        break;
    }
    return 0;
}

int validationInt(const string &text, int max)
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
        cout << text;
        cin >> angka;
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