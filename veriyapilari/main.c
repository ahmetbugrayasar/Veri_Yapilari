//
//  main.c
//  veriyapilari
//
//  Created by Ahmet Buğra Yaşar on 14.10.2022.
//

#include <stdio.h>
#include<stdlib.h>
#include<time.h>

struct dugum{
    int anahtar;
    struct dugum* sonraki;
};

/// Verilen anahtar değerinde yeni bir düğüm oluşturur
struct dugum *dugum_olustur(int anahtar){
    //Oluşacak düğümün adresini tutan bir a değişkeni oluşturalım
    struct dugum* a;
    
    //a'ya düğüm'ün heapteki yeri kadar (int + dugum*) yer ayıralım. Malloc void* döndürdüğü için dugum* değişkenine cast yapalım.
    a = (struct dugum*)malloc(sizeof(struct dugum));
    
    //Eğer malloc fonksiyonu geçersiz olmuşsa
    if(a == NULL) {
        printf("Yer ayrılamadı!");
        exit(1);
    }
    //Eğer olmamışsa, yeni a düğümünün anahtarı, parametre'deki anahtar
    a->anahtar = anahtar;
    //a'dan sonraki düğüme işaret eden sonraki pointer'ı da boş. (a son düğüm)
    a-> sonraki = NULL;
    
    return a;
}

//Anahtar değerine sahip yeni bir düğüm oluşturur, liste_basi pointer'ını a'nın adresine tutarak ve a'dan sonrakini, a yaratılmadan önceki liste başına tutarak onu listenin başına koyar.
void liste_basina_ekle(int anahtar, struct dugum **liste_basi)
{
    //anahtar değerinde yeni bir düğüm oluşturur.
    struct dugum *a = dugum_olustur(anahtar);
    
    //a'dan sonrakini, liste başı olarak işaretler
    a-> sonraki = *liste_basi;
    
    //Liste başı'nı a'ya tutturur.
    *liste_basi = a;
}

void liste_sonuna_ekle(int anahtar, struct dugum** liste_basi){
    //anahtar değerinde yeni bir düğüm oluşturur.
    struct dugum *a = dugum_olustur(anahtar);
    //Eğer liste boşsa, liste_basi boş bir değer tutar. Bu da, a'yı tek değer yapacağı için, liste başında olmuş olur.
    if(*liste_basi == NULL)
    {
        //Liste başı hali hazırda boş bir değer tuttuğu için, a'dan sonraki adresi liste başının gösterdiği adrese tutarız.
        a->sonraki = *liste_basi;
        
        //Liste başını da a'ya tutarak, a'yı listeye almış ve listenin başı yapmış oluruz.
        *liste_basi = a;
    }
    
    else{
        //Listede dolaşacak bir dugum pointerı lazım. Bu pointer listenin başından başlamalı.
        struct dugum* temp = *liste_basi;
        
        
        //oluşturduğum bu gezgin düğüm pointerı, liste başından başlayarak düğümlerin sonraki pointerlarının değerlerine bakar. Eğer değer boşsa, listenin sonuna gelmiştir. Eğer değer doluysa, kendi sonrakini o düğümün sonrakine tutar.
        while(temp->sonraki != NULL)
        {
            temp = temp->sonraki;
        }
        temp -> sonraki = a;
    }
}

/* Bu fonskiyon, yeni düğümün sıralamalı şekilde (küçükten büyüğe gibi) girebilmesi içindir. Kendi anahtarından daha büyük anahtara sahip ilk düğümden hemen önce yerleştirir.
 
 Kaplumbağa ve Tavşan değişkenleri, düğümler arasında giden dugum pointerlarıdır. Amaçları, hangi iki düğümün arasına gireceğine karar vermektir.
 Kaplumbağa, daima tavşanın 1 adım gerisindedir. Uygun bir yer bulan eklenecek düğüm, kaplumbağanın işaret ettiği düğümün sonraki düğümü olur. Eklenecek düğümün sonrakisi de Tavşan'ın tuttuğu değere işaret eder.
 */
void liste_sirali_ekle(int anahtar, struct dugum** liste_basi){
    /*Geriden gelen dugumun işaretçisi => kaplumbaga
     Önden giden dugumun işaretçisi => Tavşan
     Parametreden verilen anahtara sahip dugumun işaretçisi eklenecek
     */
    struct dugum *kaplumbaga = NULL, *tavsan, *eklenecek;
    
    //Önce tavşanı liste_basi pointerının işaret ettiği düğüme (Listedeki ilk düğüme) işaret ettiriyoruz. Burada kaplumbağa boşta, tavşan ise listenin ilk başındadır.
    tavsan = *liste_basi;
    
    //Eğer listenin başı boşa işaret ediyorsa demek ki liste boştur. Bu durumda eklenecek düğüm ne olursa olsun yerleştirilir.
    
    //Tavşan (listenin başının işaret ettiği yere işaret ediyor şu anda) boş değilse, ve düğümünün anahtarı, eklenecek anahtardan büyük değil ise
    while(tavsan != NULL && tavsan->anahtar <= anahtar)
    {
        //Eğer tavşanın anahtar değeri, eklenecek anahtar değerine eşitse while döngüsünden çıkarız. Direkt tavşanın bulunduğu düğümle kaplumbağanın işaret ettiği düğümün arasına yerleştirilmeye gider.
        if(tavsan->anahtar == anahtar) return;
        
        //kaplumbağa tavşanın bulunduğu düğüme gelir.
        kaplumbaga = tavsan;
        //Tavşan da bir sonraki düğüme gider.
        tavsan = tavsan->sonraki;
        
        //Burada tavşan hâlâ kaplumbağadan 1 adım öndedir.
    }
    /* Ya listenin sonuna gelerek, ya da eklenecek anahtar değerinden daha büyük bir anahtar değerine rastlayarak while döngüsünden çıkarsak:
     anahtar değerine sahip bir düğüm oluşturup, eklenecek işaretçisini yeni düğüme tutarız.
     */
    eklenecek = dugum_olustur(anahtar);
    
    //Eğer ki listede tek eleman varsa, ve eklenecek anahtar değeri o tek elemanın anahtarından küçükse bu if koşuluna girer.
    if(tavsan==*liste_basi)
    {
        //Ekleneceğin tuttuğu yeni düğümü, liste'deki elemanın bir öncesine koyar.
        eklenecek->sonraki = *liste_basi;
        //ve yeni eklenen düğümü listenin başı yapar.
        *liste_basi = eklenecek;
    }
    //Eğer ki listede birden fazla eleman olduğu için bir gezinme olmuşsa, veya tek elemanlı bir listedeki anahtar, eklenecek anahtardan küçükse
    else{
        //kaplumbağa'nın tuttuğu düğümün sonraki işaretçisini yeni düğüme bağlarız.
        kaplumbaga->sonraki = eklenecek;
        //Yeni eklenen düğümün işaretçisini de tavşan'a bağlarız.
        eklenecek->sonraki = tavsan;
        
        //Bu sayede yeni fonksiyon, tavşan ve kaplumbağa arasına yerleştirilmiş olur.
    }
    
}

//Oluşturulan düğüm listesini yazdırır
void liste_yaz(struct dugum* gezgin)
{
    //gezgin pointerı boş bir adrese işaret etmedikçe
    while(gezgin != NULL)
    {
        //gezginin tuttuğu düğümün anahtarını yazdır.
        printf("%4d\n",gezgin->anahtar);
        //gezgini, sonraki düğüme işaret ettir.
        gezgin = gezgin->sonraki;
    }
}

// İlk elemandan başlayarak tek tek stackteki elemanları kaldırır. Nasıl işlediğini anlamak için: https://i.imgur.com/agOkY8T.gifv
void liste_yok_et(struct dugum** liste_basi)
{
    while(*liste_basi != NULL){
        printf("\nListe Başı eleman siliniyor: %d \n",(*liste_basi)->anahtar);
        free(*liste_basi);
        
        *liste_basi = (*liste_basi)->sonraki;
    }
}

//Listeden eleman silmek için 2 bilgiye ihtiyacımız var: Silinecek dugumun değeri ve listenin ilk elemanı.
void liste_eleman_sil(int silinecek, struct dugum** liste_basi)
{
    //Yukarıdaki tavşan ve kaplumbağa örneğine benzer bir durum olarak temp ve once değişkenlerine sahibiz. temp, önceden ileride hareket eder. Başlangıç değeri, listenin ilk elemanıdır.
    struct dugum* temp = *liste_basi;
    struct dugum* once;
    
    //eğer listenin ilk elemanı olarak atandıktan sonra, temp'in değeri hâlâ null ise, demek ki liste boştur.
    //Eğer ki tempin anahtarı, aranan anahtarsa, aramanın bir anlamı yok. Döngüden çıkabiliriz, aradığımız düğümü bulduk.
    while(temp!= NULL && temp->anahtar != silinecek)
    {
        //Bulamadığımız sürece aramak için, once dugumunu temp'e, temp dugumunu de bir ileriki dugume işaretleriz.
        once = temp;
        temp = temp->sonraki;
    }
    if(temp == NULL) return;
    
    else{
        //Eğer arama sonucunda direkt ilk elemandan aradığımız değeri bulursak, direkt liste başını bir ileriki düğüme götürüp aradığımız hücreyi görünürden kaldırırız. Eğer tek hücre varsa bu zaten sonraki olarak NULL'a işaret edeceği için yine bir sıkıntı göstermez.
        if(temp ==*liste_basi)
        {
            
            *liste_basi = (*liste_basi)->sonraki;
            free(temp);
        }
        else{
            once->sonraki = temp->sonraki;
            free(temp);
        }
    }
}

void liste_daha_kucuk_eleman_cikar(int filtre_sayisi, struct dugum** liste_basi)
{
    struct dugum *tavsan = *liste_basi;
    struct dugum *kaplumbaga = *liste_basi;
    
    while(tavsan != NULL)
    {
        if(filtre_sayisi > tavsan->anahtar)
        {
            if(tavsan == *liste_basi)
            {
                *liste_basi = (*liste_basi)->sonraki;
                free(tavsan);
                tavsan = kaplumbaga = *liste_basi;
            }
            else{
                kaplumbaga->sonraki = tavsan->sonraki;
                free(tavsan);
                tavsan = kaplumbaga->sonraki;
            }
        }
        else{
            kaplumbaga = tavsan;
            tavsan = tavsan->sonraki;
        }
        
    }
}

int ardisik_mi(struct dugum* liste_basi, int anahtar1, int anahtar2)
{
    
    struct dugum* tavsan = NULL;
    
    while(liste_basi!= NULL)
    {
        if(liste_basi == anahtar1 || liste_basi == anahtar2 ) break;
        tavsan = liste_basi;
        liste_basi = liste_basi->sonraki;
    }
    if(liste_basi !=NULL && tavsan != NULL && (liste_basi->anahtar == anahtar1 && tavsan == anahtar2)){
        return 1;
    }
    return 0;
}

int ardisik_mi_tek_degisken(struct dugum* liste_basi, int anahtar1, int anahtar2)
{
    if(liste_basi->sonraki == NULL){
        printf("Liste tek elemanlı!\n");
        return 0;
    }
    while(liste_basi!= NULL)
    {
        if(liste_basi->anahtar == anahtar1 || liste_basi->anahtar == anahtar2) break;
        liste_basi = liste_basi->sonraki;
    }
    if((liste_basi != NULL && liste_basi->sonraki != NULL)
       &&
       ((liste_basi->anahtar == anahtar1 && liste_basi->sonraki->anahtar == anahtar2)
       ||
       (liste_basi->anahtar == anahtar2 && liste_basi->sonraki->anahtar == anahtar1)))
    {
        printf("\nArdisik\n");
        return 1;
    }
    printf("Ardisik degil!");
    return 0;
}
//Recursive fonksiyonlar, kendilerini yeni parametrelerle çağıran memurumtırak fonksiyonlardır.
/*
 Gezgin, en sondaki elemandan sonraki boşluğa gelene kadar:
    1-) Gezgin'in işaret ettiği düğümün anahtar değerini yazdır
    2-) Gezginden sonraki düğümle tekrar fonksiyonu çağır
 Eğer Gezgin boşsa:
    yeni satıra geç. (Estetik amaçlı)
 */

/*
    ############################# RECURSIVE FONKSİYONLARIN MANTIĞI #############################
 
    Recursive fonksiyonlar, fonksiyonda verilen koşul tamamlanana kadar, yeni bir parametre ile kendilerini çağırırlar. Aksi takdirde sonsuza kadar çalışırlar ve döngüye girerler.
 
    --------------------------------------------------------------------------------------------
    Bu fonksiyonun nasıl çalıştığını anlamak için recursive fonksiyonların nasıl verileri
    stack'te tuttuğunu anlamamız gerekiyor:
 
    Normal fonksiyonlar, main'den çağrılmaları kesildikten sonra, içlerindeki değerleri silerler. Recursive fonksiyonların bu çağrılmaları ise, fonksiyonda verilen koşul tamamlanana kadar silinmez. Yani iç içe çağrılan fonksiyonların stack değerleri, ilk başta çağrılan iterasyon silinene kadar silinmez.
    Birbirlerine yapışık baloncuk gibi hayal edebilirsiniz. En son koyduğunuz balon (koşullar sağlanınca) patlayıp ondan önceki baloncukları da patlatmaya başlar.
    
 */

void recursive_liste_yaz(struct dugum* gezgin)
{
    //Eğer gezgin NULL ise, listenin sonuna gelinmiştir.
    if(gezgin != NULL)
    {
        //Gezginin anahtarını yazdırır.
        printf("%d",gezgin->anahtar);
        //Aynı fonksiyonu, bu sefer gezgin'den sonraki düğüm ile başlatır.
        recursive_liste_yaz(gezgin->sonraki);
    }
    else{
        printf("\n");
    }
}


void tersten_liste_yaz_recursive(struct dugum* gezgin)
{
    if(gezgin != NULL)
    {
        tersten_liste_yaz_recursive(gezgin->sonraki);
        printf("%d",gezgin->anahtar);
    }
}

int main(int argc, const char * argv[]) {
    struct dugum* liste = NULL;
    liste_basina_ekle(5, &liste);
    liste_basina_ekle(55, &liste);
    liste_basina_ekle(25, &liste);
    
    
    liste_yaz(liste);
    liste_yok_et(&liste);
    

    liste_basina_ekle(12, &liste);
    liste_sirali_ekle(4, &liste);
    liste_sirali_ekle(5, &liste);
    liste_sirali_ekle(6, &liste);
    
    
    liste_yaz(liste);
    
    ardisik_mi_tek_degisken(liste, 4, 5);
    return 0;
}
