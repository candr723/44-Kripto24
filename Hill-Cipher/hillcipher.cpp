#include <iostream>
#include <vector>
#include <string>
#include <cctype>

using namespace std;

vector<vector<int>> InputKunci(int n);
vector<int> StringConverter(const string &P);
string IntConverter(const vector<int> &C);
vector<vector<int>> Invers(const vector<vector<int>> &K, int n);
vector<vector<int>> Adjoint(const vector<vector<int>> &K, int n);
int Determinan(const vector<vector<int>> &K, int n);
int ModInvers(int a, int m);
vector<int> MatrixMultiply(const vector<vector<int>> &K, const vector<int> &plain, int n);
void enc(const string &P, int n);
void dec(const string &P, int n);
void kunci(const string &pt, const string &ct, int n);
void kofaktor(const vector<vector<int>> &K, vector<vector<int>> &temp, int p, int q, int n);

void enc(const string &P, int n)
{
    string plaintext = P;
    for (char &c : plaintext)
    {
        c = toupper(c);
    }

    int size = (plaintext.size() + n - 1) / n * n;
    plaintext.resize(size, 'X'); 

    vector<int> C(size, 0);
    vector<vector<int>> K = InputKunci(n);
    vector<int> plain = StringConverter(plaintext);

    cout << "Plaintext (numerical): ";
    for (int p : plain)
    {
        cout << p << " ";
    }
    cout << endl;

    for (int i = 0; i < size; i += n)
    {
        vector<int> block(n, 0);
        for (int j = 0; j < n; j++)
        {
            block[j] = plain[i + j];
        }
        vector<int> temp = MatrixMultiply(K, block, n);
        for (int j = 0; j < n; j++)
        {
            C[i + j] = (temp[j] % 26 + 26) % 26 + 65; 
        }
    }

    string cipher = IntConverter(C);
    cout << "Ciphertext: " << cipher << endl;
}

void dec(const string &P, int n)
{
    vector<vector<int>> K = InputKunci(n);
    vector<vector<int>> K_inv = Invers(K, n);

    vector<int> plain(P.size(), 0);
    vector<int> cipher = StringConverter(P);

    for (int i = 0; i < P.size(); i += n)
    {
        vector<int> block(n, 0);
        for (int j = 0; j < n; j++)
        {
            block[j] = cipher[i + j];
        }
        vector<int> temp = MatrixMultiply(K_inv, block, n);
        for (int j = 0; j < n; j++)
        {
            plain[i + j] = (temp[j] % 26 + 26) % 26 + 65; 
        }
    }

    string decrypted = IntConverter(plain);
    cout << "Plaintext: " << decrypted << endl;
}

vector<vector<int>> InputKunci(int n)
{
    vector<vector<int>> K(n, vector<int>(n));

    cout << "Input Matriks kunci (size " << n << "x" << n << "):" << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << "K[" << i << "][" << j << "]: ";
            cin >> K[i][j];
        }
    }

    return K;
}

vector<int> StringConverter(const string &P)
{
    vector<int> plain;
    for (char c : P)
    {
        if (isalpha(c))
        {
            plain.push_back(toupper(c) - 65); 
        }
    }
    return plain;
}

string IntConverter(const vector<int> &C)
{
    string cipher(C.size(), ' ');
    for (int i = 0; i < C.size(); i++)
    {
        cipher[i] = static_cast<char>(C[i]); 
    }
    return cipher;
}

vector<vector<int>> Invers(const vector<vector<int>> &K, int n)
{
    int det = Determinan(K, n);
    int invDet = ModInvers(det, 26);
    vector<vector<int>> adj = Adjoint(K, n);
    vector<vector<int>> inv(n, vector<int>(n));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            inv[i][j] = (adj[i][j] * invDet) % 26;
            if (inv[i][j] < 0)
                inv[i][j] += 26;
        }
    }
    return inv;
}

vector<vector<int>> Adjoint(const vector<vector<int>> &K, int n)
{
    vector<vector<int>> adj(n, vector<int>(n));
    int sign = 1;
    vector<vector<int>> temp(n, vector<int>(n));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            kofaktor(K, temp, i, j, n);
            sign = ((i + j) % 2 == 0) ? 1 : -1;
            adj[j][i] = (sign * Determinan(temp, n - 1)) % 26;
            if (adj[j][i] < 0)
                adj[j][i] += 26;
        }
    }
    return adj;
}

int Determinan(const vector<vector<int>> &K, int n)
{
    int det = 0;
    if (n == 1)
        return K[0][0];
    if (n == 2)
        return (K[0][0] * K[1][1] - K[0][1] * K[1][0]) % 26;
    vector<vector<int>> temp(n, vector<int>(n));
    int sign = 1;

    for (int f = 0; f < n; f++)
    {
        kofaktor(K, temp, 0, f, n);
        det = (det + sign * K[0][f] * Determinan(temp, n - 1)) % 26;
        if (det < 0)
            det += 26;
        sign = -sign;
    }
    return det;
}

void kofaktor(const vector<vector<int>> &K, vector<vector<int>> &temp, int p, int q, int n)
{
    int i = 0, j = 0;
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            if (row != p && col != q)
            {
                temp[i][j++] = K[row][col];
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

int ModInvers(int a, int m)
{
    int m0 = m, t, q;
    int x0 = 0, x1 = 1;

    if (m == 1)
        return 0;

    while (a > 1)
    {
        q = a / m;
        t = m;
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0)
        x1 += m0;

    return x1;
}

vector<int> MatrixMultiply(const vector<vector<int>> &K, const vector<int> &plain, int n)
{
    vector<int> result(n, 0);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            result[i] += K[i][j] * plain[j];
        }
        result[i] = (result[i] % 26 + 26) % 26;
    }
    return result;
}

void kunci(const string &pt, const string &ct, int n)
{
    vector<int> plain = StringConverter(pt);
    vector<int> cipher = StringConverter(ct);
    if (plain.size() != cipher.size() || plain.size() % n != 0)
    {
        cout << "Ukuran tidak sama atau tidak kelipatan dari ukuran matriks" << endl;
        return;
    }

    vector<vector<int>> M(n, vector<int>(n, 0));

    for (size_t i = 0; i < plain.size(); i += n)
    {
        vector<int> pt(n, 0);
        vector<int> ct(n, 0);

        for (int j = 0; j < n; j++)
        {
            pt[j] = plain[i + j];
            ct[j] = cipher[i + j];
        }

        vector<vector<int>> tempMatrix(n, vector<int>(n, 0));

        for (int r = 0; r < n; r++)
        {
            for (int c = 0; c < n; c++)
            {
                tempMatrix[r][c] = (ct[r] * ModInvers(pt[c], 26)) % 26;
                if (tempMatrix[r][c] < 0)
                    tempMatrix[r][c] += 26;
            }
        }

        M = tempMatrix;
    }

    cout << "Matriks kunci ditemukan: " << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << M[i][j] << " ";
        }
        cout << endl;
    }
}

int main()
{
    int opsi, n;
    string P, pt, ct;

    cout << "Pilih Program \n 1. Enkripsi \n 2. Dekripsi \n 3. Cari Kunci" << endl;
    cin >> opsi;

    switch (opsi)
    {
    case 1:
        cout << "Masukan Ukuran Matriks Kunci: ";
        cin >> n;
        cout << "Masukan Plaintext: ";
        cin.ignore();
        getline(cin, P);
        enc(P, n);
        break;

    case 2:
        cout << "Masukan Ukuran Matriks Kunci: ";
        cin >> n;
        cout << "Masukan Ciphertext: ";
        cin.ignore();
        getline(cin, P);
        dec(P, n);
        break;

    case 3:
        cout << "Masukan Ukuran Matriks Kunci: ";
        cin >> n;
        cout << "Masukan Plaintext: ";
        cin.ignore();
        getline(cin, pt);
        cout << "Masukan Ciphertext: ";
        getline(cin, ct);
        kunci(pt, ct, n);
        break;

    default:
        cout << "Opsi Tidak Ada" << endl;
    }

    return 0;
}