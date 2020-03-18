#include<stdlib.h>
#include<stdio.h>
#include<string>
struct MasinaAprovizionare {
	int nrKm;
	char* denumireAuto;
	float pretAchizitie;
};

struct Simigerie {
	char* locatie;
	MasinaAprovizionare* m;
	float chirie;
	int nrProduse;
};

Simigerie initSimigerie(const char* locatie, MasinaAprovizionare* m, float chirie, int nrProduse) {
	Simigerie s;
	s.locatie = (char*)malloc(sizeof(char) * (strlen(locatie) + 1));
	strcpy(s.locatie, locatie);
	s.chirie = chirie;
	s.nrProduse = nrProduse;
	s.m = (MasinaAprovizionare*)malloc(sizeof(MasinaAprovizionare));
	s.m = m;
	return s;
}

struct Nod {
	Simigerie info;
	Nod* next;
};

MasinaAprovizionare initMasinaAprovizionare(const char* denumireAuto, int nrKm, float pretAchizitie) {
	MasinaAprovizionare masina;
	masina.denumireAuto = (char*)malloc(sizeof(char) * (strlen(denumireAuto) + 1));
	strcpy(masina.denumireAuto, denumireAuto);
	masina.nrKm = nrKm;
	masina.pretAchizitie = pretAchizitie;
	return masina;
}

void afisareMasina(MasinaAprovizionare masina) {
	printf("Masina %s are %d mii KM si a costat %5.2f euro", masina.denumireAuto, masina.nrKm, masina.pretAchizitie);
}

void afisareSimigerie(Simigerie s) {
	printf("Simigeria locata la adresa '%s' plateste o chirie de %5.2f euro si are %d produse si are urmatoarea flota de masini:\n", s.locatie, s.chirie, s.nrProduse);
	for (int i = 1; i <= s.m[0].nrKm; ++i) {
		//printf("%d", (s.m)[7].nrKm);
		printf("%d ->", i);
		afisareMasina(s.m[i]);
		printf("\n");
	}
}

void adaugareNodInListaSimpla(Nod*& cap, Simigerie s) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = s;
	if (cap) {
		Nod* aux = cap;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = nou;
		nou->next = NULL;
		//nou->next = cap;
		//cap = nou;
	}
	else {
		cap = nou;
		cap->next = NULL;
	}
}

void afisareListaSimpla(Nod* cap) {
	if (!cap) {
		printf("LISTA E GOALA SAU A FOST STEARSA!\n\n");
	}
	while (cap) {
		afisareSimigerie(cap->info);
		printf("\n");
		cap = cap->next;
	}
}

MasinaAprovizionare* initFlotaMasiniAprovizionare(MasinaAprovizionare*& flota_Simigerie, int nrMasini) {
	MasinaAprovizionare* nou = (MasinaAprovizionare*)malloc(sizeof(MasinaAprovizionare) * (nrMasini + 1));
	nou[0].nrKm = nrMasini; //Am stocat numarul de masini ca sa le pot afisa mai tarziu
	return nou;
}

void stergereListaSimpla(Nod*& cap) {
	while (cap) {
		Nod* aux = cap;
		cap = cap->next;
		for (int i = 1; i <= aux->info.m[0].nrKm; i++) {
			free(aux->info.m[i].denumireAuto);
		}
		free(aux->info.m);
		free(aux->info.locatie);
		free(aux);

	}
	//cap = NULL;
}

// Filtrarea va lua exclusiv Simigeriile a caror flota de masini nu contine vreo masina cu pretul de achizitie = 0
Simigerie* filtrare(Nod* cap, int& marimeVector) {
	if (cap) {
		int nrSimigerii = 0;
		Nod* aux = cap;
		while (aux) {
			nrSimigerii++;
			aux = aux->next;
		}
		Simigerie* vectorSimigerii = (Simigerie*)malloc(sizeof(Simigerie) * nrSimigerii);
		int j = 0;
		while (cap && j < nrSimigerii) {
			int contorNrAutoPerSimigerie = 0;
			for (int i = 1; i <= cap->info.m[0].nrKm; i++) { // in cap->info.m[0].nrKm este stocat numarul de masini al acestui vector de masini si am si cateva intrebari referitoare la best-practice
				if (cap->info.m[i].pretAchizitie == 0) break;
				else {
					contorNrAutoPerSimigerie++;
				}
			}
			if (contorNrAutoPerSimigerie == cap->info.m[0].nrKm)
			{
				vectorSimigerii[j] = cap->info;
				j++;
				cap = cap->next;
			}
			else
				cap = cap->next;
		}
		marimeVector = j;
		if (j < nrSimigerii) {
			Simigerie* vectorNou = (Simigerie*)malloc(sizeof(Simigerie) * j);
			for (int i = 0; i < j; i++) {
				vectorNou[i].m = initFlotaMasiniAprovizionare(vectorNou[i].m, vectorSimigerii[i].m[0].nrKm);
				for (int k = 1; k <= vectorSimigerii[i].m[0].nrKm; k++) {
					vectorNou[i].m[k] = initMasinaAprovizionare(
						vectorSimigerii[i].m[k].denumireAuto,
						vectorSimigerii[i].m[k].nrKm,
						vectorSimigerii[i].m[k].pretAchizitie);
				}
				vectorNou[i].chirie = vectorSimigerii[i].chirie;
				vectorNou[i].locatie = (char*)malloc(sizeof(char) * (strlen(vectorSimigerii[i].locatie) + 1));
				strcpy(vectorNou[i].locatie, vectorSimigerii[i].locatie);
				vectorNou[i].chirie = vectorSimigerii[i].chirie;
				vectorNou[i].nrProduse = vectorSimigerii[i].nrProduse;
			}

			free(vectorSimigerii);
			return vectorNou;
		}
		else
			return vectorSimigerii;
	}
	else {
		return NULL;
	}
}

void afisareVectorSimigerii(Simigerie* vectorSimigerii, int nrElemente) {
	int i = 0;
	while (i < nrElemente) {
		afisareSimigerie(vectorSimigerii[i]);
		printf("\n");
		i++;
	}
}

void main() {
	MasinaAprovizionare* flota_Simigerie1 = initFlotaMasiniAprovizionare(flota_Simigerie1, 4);
	flota_Simigerie1[1] = initMasinaAprovizionare("Dacia LOGHAN", 144567, 5.2);
	flota_Simigerie1[2] = initMasinaAprovizionare("Lamborghini Huracan", 14000, 20);
	flota_Simigerie1[3] = initMasinaAprovizionare("BMW i3 Model CAZAN", 350000, 3850);
	flota_Simigerie1[4] = initMasinaAprovizionare("Husqvarna X6 Solenza", 1500, 145000);

	Simigerie simigerie1 = initSimigerie("B-dul Anghel Saligny nr.2", flota_Simigerie1, 450.12, 69);


	MasinaAprovizionare* flota_Simigerie2 = initFlotaMasiniAprovizionare(flota_Simigerie2, 4);
	flota_Simigerie2[1] = initMasinaAprovizionare("Dacia Solenza", 144567, 5.2);
	flota_Simigerie2[2] = initMasinaAprovizionare("Peugeot 452", 14000, 800);
	flota_Simigerie2[3] = initMasinaAprovizionare("Oreo", 350000, 0);
	flota_Simigerie2[4] = initMasinaAprovizionare("Coca Cola", 1500, 145000);
	Simigerie simigerie2 = initSimigerie("Str.Anton Pann nr.4B", flota_Simigerie2, 450.12, 69);


	MasinaAprovizionare* flota_Simigerie3 = initFlotaMasiniAprovizionare(flota_Simigerie3, 4);
	flota_Simigerie3[1] = initMasinaAprovizionare("Spirt", 144567, 5.2);
	flota_Simigerie3[2] = initMasinaAprovizionare("Sprite", 14000, 240);
	flota_Simigerie3[3] = initMasinaAprovizionare("Dezinfectant", 350000, 2500);
	flota_Simigerie3[4] = initMasinaAprovizionare("Restantierilor", 1500, 145000);

	Simigerie simigerie3 = initSimigerie("Str.Adrian nr.174A", flota_Simigerie3, 450.12, 69);

	MasinaAprovizionare* flota_Simigerie4 = initFlotaMasiniAprovizionare(flota_Simigerie4, 6);
	flota_Simigerie4[1] = initMasinaAprovizionare("Spirt", 144567, 5.2);
	flota_Simigerie4[2] = initMasinaAprovizionare("Sprite", 14000, 300);
	flota_Simigerie4[3] = initMasinaAprovizionare("Dezinfectant", 350000, 2500);
	flota_Simigerie4[4] = initMasinaAprovizionare("Restantierilor", 1500, 145000);
	flota_Simigerie4[5] = initMasinaAprovizionare("Restantierilor1", 345755, 12800);
	flota_Simigerie4[6] = initMasinaAprovizionare("Restantierilor2", 28560, 246888);

	Simigerie simigerie4 = initSimigerie("B-dul Dacia nr.4 Bl.Y3A sc.1", flota_Simigerie4, 350, 12);

	Nod* cap = NULL;
	adaugareNodInListaSimpla(cap, simigerie1);
	adaugareNodInListaSimpla(cap, simigerie2);
	adaugareNodInListaSimpla(cap, simigerie3);
	adaugareNodInListaSimpla(cap, simigerie4);

	afisareListaSimpla(cap);

	printf("\n\n");
	printf("Dupa filtrare:\n");

	int marimeVector = 0;
	Simigerie* vectorSimigerii = filtrare(cap, marimeVector);
	printf("%d\n", marimeVector);

	stergereListaSimpla(cap);
	afisareListaSimpla(cap);
	afisareVectorSimigerii(vectorSimigerii, marimeVector); // A fost folosit DEEP-CPY astfel ca vectorul persista in eventualitatea stergerii listei.

}