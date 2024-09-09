// Candra Wibawa
// 140810220044

#include <iostream>
using namespace std;

string encrypt(string text, int k){
    string hasil = "";

    for (int i = 0; i < text.length(); i++)
    {
        if (isupper(text[i]))
            hasil += char(int(text[i] + k - 65) % 26 + 65);

        else if (islower(text[i]))
        hasil += char(int(text[i] + k - 97) % 26 + 97);

        else 
        hasil += text[i];
    }
    return hasil;
}

string decrypt(string text, int k){
    string hasil = "";

    for (int i = 0; i < text.length(); i++)
    {
        if (isupper(text[i]))
            hasil += char(int(text[i] - k - 65 + 26) % 26 + 65);

        else if(islower(text[i]))
            hasil += char(int(text[i] - k - 97 + 26) % 26 + 97);

        else
        hasil += text[i];
    }
    return hasil;
    
}

void menu(){
    int pilihan;
    int k;
    string text;
    cout << "Selamat datang di shift cipher\n";
    cout << "1. Enkripsi\n2. Dekripsi\n";
    cout << "Pilih program : ";
    cin >> pilihan;
    cin.ignore();
    switch (pilihan)
    {
    case 1:
        cout << "Masukkan plain text untuk di enkripsi : ";
        getline(cin, text);
        cout << endl;
        cout << "Masukkan key : ";
        cin >> k;
        cout << encrypt(text, k);
        break;
    
    case 2:
        cout << "Masukkan cipher text untuk di enkripsi : ";
        getline(cin, text);
        cout << endl;
        cout << "Masukkan key : ";
        cin >> k;
        cout << decrypt(text, k);
        break;
    
    default:
        menu();
    }
}

int main(){
    menu();
}