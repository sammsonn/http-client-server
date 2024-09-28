## PCOM - Tema 4
## Samson Alexandru-Valentin
## 322CC

---

# Biblioteca de parsare JSON

Am ales sa folosesc biblioteca parson deoarece este recomandata in cerinta
pentru limbajul C si este usoara de inteles si folosit.

---

# Functia register_client

Implementare:

- Citeste username-ul si parola de la tastatura. Daca acestea contin spatii sau
  sunt goale, afiseaza un mesaj de eroare si iese din functie.
- Verifica daca utilizatorul este deja inregistrat verificand daca cookie-ul
  este gol. Daca nu este gol, afiseaza un mesaj de eroare si iese din functie.
- Creeaza un obiect JSON cu username-ul si parola. Acesta va fi folosit in
  corpul cererii POST.
- Serializeaza obiectul JSON intr-un string.
- Creeaza mesajul de cerere POST folosind functia compute_post_request. Acesta
  include IP-ul host-ului, calea endpoint-ului, tipul de continut (in acest caz,
  "application/json"), string-ul serializat, si alte informatii necesare pentru
  cererea HTTP.
- Trimite cererea la server folosind functia send_to_server.
- Primeste raspunsul de la server folosind functia receive_from_server.
- Verifica daca username-ul este deja luat verificand daca raspunsul contine
  cuvantul "error". Daca da, afiseaza un mesaj de eroare. Altfel, afiseaza un
  mesaj de succes.
- Elibereaza memoria alocata pentru string-ul serializat, mesajul de cerere,
  raspunsul de la server, si obiectul JSON.

---

# Functia login_client

Implementare:

- Citeste username-ul si parola de la tastatura. Daca acestea contin spatii sau
  sunt goale, afiseaza un mesaj de eroare si iese din functie.
- Verifica daca utilizatorul este deja logat verificand daca cookie-ul este gol.
  Daca nu este gol, afiseaza un mesaj de eroare si iese din functie.
- Creeaza un obiect JSON cu username-ul si parola. Acesta va fi folosit in
  corpul cererii POST.
- Serializeaza obiectul JSON intr-un string.
- Creeaza mesajul de cerere POST folosind functia compute_post_request. Acesta
  include IP-ul host-ului, calea endpoint-ului, tipul de continut (in acest caz,
  "application/json"), string-ul serializat, si alte informatii necesare pentru
  cererea HTTP.
- Trimite cererea la server folosind functia send_to_server.
- Primeste raspunsul de la server folosind functia receive_from_server.
- Verifica daca username-ul sau parola sunt incorecte verificand daca raspunsul
  contine cuvantul "error". Daca da, afiseaza un mesaj de eroare. Altfel, extrage
  cookie-ul din raspunsul serverului si il salveaza in parametrul cookie, apoi
  afiseaza un mesaj de succes.
- Elibereaza memoria alocata pentru string-ul serializat, mesajul de cerere,
  raspunsul de la server, si obiectul JSON.

---

# Functia enter_library

Implementare:

- Verifica daca utilizatorul a intrat deja in biblioteca verificand daca
  token-ul este gol. Daca nu este gol, afiseaza un mesaj de eroare si iese
  din functie.
- Creeaza mesajul de cerere GET folosind functia compute_get_request. Acesta
  include IP-ul host-ului, calea endpoint-ului, cookie-ul, si alte informatii
  necesare pentru cererea HTTP.
- Trimite cererea la server folosind functia send_to_server.
- Primeste raspunsul de la server folosind functia receive_from_server.
- Verifica daca utilizatorul este logat verificand daca raspunsul contine
  cuvantul "error". Daca da, afiseaza un mesaj de eroare. Altfel, extrage
  token-ul din raspunsul serverului si il salveaza in parametrul token, apoi
  afiseaza un mesaj de succes.
- Elibereaza memoria alocata pentru mesajul de cerere si raspunsul de la
  server.

---

# Functia get_books

Implementare:

- Verifica daca utilizatorul este logat verificand daca cookie-ul este gol.
  Daca este gol, afiseaza un mesaj de eroare si iese din functie.
- Creeaza mesajul de cerere GET folosind functia compute_get_request. Acesta
  include IP-ul host-ului, calea endpoint-ului, cookie-ul, token-ul, si alte
  informatii necesare pentru cererea HTTP.
- Trimite cererea la server folosind functia send_to_server.
- Primeste raspunsul de la server folosind functia receive_from_server.
- Verifica daca utilizatorul are acces la biblioteca verificand daca raspunsul
  contine cuvantul "error". Daca da, afiseaza un mesaj de eroare. Altfel,
  extrage inceputul JSON-ului din raspunsul serverului.
- Parseaza raspunsul ca JSON. Daca parsarea esueaza, afiseaza un mesaj de
  eroare si iese din functie.
- Obtine array-ul de carti din JSON. Daca array-ul nu exista, afiseaza un
  mesaj de eroare si iese din functie.
- Afiseaza cartile. Pentru fiecare carte din array, afiseaza ID-ul si titlul.
- Elibereaza memoria alocata pentru mesajul de cerere si raspunsul de la
  server.

---

# Functia get_book

Implementare:

- Citeste ID-ul cartii de la utilizator. Daca ID-ul este gol sau contine
  caractere care nu sunt cifre, afiseaza un mesaj de eroare si iese din functie.
- Primeste raspunsul de la server folosind functia receive_from_server.
- Verifica daca cartea exista verificand daca raspunsul contine cuvantul "error".
  Daca da, afiseaza un mesaj de eroare. Altfel, extrage inceputul JSON-ului din
  raspunsul serverului.
- Parseaza raspunsul ca JSON. Daca parsarea esueaza, afiseaza un mesaj de eroare
  si iese din functie.
- Obtine obiectul carte din JSON. Daca obiectul nu exista, afiseaza un mesaj de
  eroare si iese din functie.
- Afiseaza detaliile cartii, inclusiv ID-ul, titlul, autorul, editorul, genul,
  si numarul de pagini.
- Elibereaza memoria alocata pentru URL, mesajul de cerere, si raspunsul de la
  server.

---

# Functia add_book

Implementare:

- Citeste informatiile despre carte de la utilizator. Daca oricare dintre
  informatii este gol sau numarul de pagini contine caractere care nu sunt cifre,
  afiseaza un mesaj de eroare si iese din functie.
- Creeaza un obiect JSON cu informatiile despre carte folosind functiile
  json_object_set_string si json_object_set_number.
- Serializeaza obiectul JSON intr-un string.
- Creeaza mesajul de cerere POST folosind functia compute_post_request. Acesta
  include IP-ul host-ului, URL-ul, tipul de continut, stringul serializat, si
  token-ul.
- Trimite cererea la server folosind functia send_to_server.
- Primeste raspunsul de la server folosind functia receive_from_server.
- Verifica daca cartea a fost adaugata cu succes verificand daca raspunsul
  contine cuvantul "error". Daca da, afiseaza un mesaj de eroare. Altfel,
  afiseaza un mesaj de succes.
- Elibereaza memoria alocata pentru stringul serializat, mesajul de cerere,
  raspunsul de la server, si obiectul JSON.

---

# Functia delete_book

Implementare:

- Citeste ID-ul cartii de la utilizator. Daca ID-ul este gol sau contine
  caractere care nu sunt cifre, afiseaza un mesaj de eroare si iese din functie.
- Verifica daca utilizatorul este logat verificand daca cookie-ul este gol.
  Daca este gol, afiseaza un mesaj de eroare si iese din functie.
- Creeaza URL-ul pentru cererea DELETE, care include ID-ul cartii.
- Creeaza mesajul de cerere DELETE folosind functia compute_delete_request.
  Acesta include IP-ul host-ului, URL-ul, si token-ul.
- Trimite cererea la server folosind functia send_to_server.
- Primeste raspunsul de la server folosind functia receive_from_server.
- Verifica daca cartea a fost stearsa cu succes verificand daca raspunsul
  contine cuvantul "error". Daca da, afiseaza un mesaj de eroare. Altfel,
  afiseaza un mesaj de succes.
- Elibereaza memoria alocata pentru URL, mesajul de cerere, si raspunsul de
  la server.

---

# Functia logout_client

Implementare:

- Creeaza mesajul de cerere GET folosind functia compute_get_request. Acesta
  include IP-ul host-ului, calea endpoint-ului, cookie-ul, si alte informatii
  necesare pentru cererea HTTP.
- Trimite cererea la server folosind functia send_to_server.
- Primeste raspunsul de la server folosind functia receive_from_server.
- Verifica daca logout-ul a fost realizat cu succes verificand daca raspunsul
  contine cuvantul "error". Daca da, afiseaza un mesaj de eroare. Altfel,
  afiseaza un mesaj de succes si seteaza cookie-ul si token-ul la NULL.
- Elibereaza memoria alocata pentru mesajul de cerere si raspunsul de la
  server.

---

# Functia main

Implementare:

- Seteaza IP-ul host-ului si deschide conexiunea cu serverul folosind functia
  open_connection.
- Aloca memorie pentru actiune, cookie si token.
- Initializeaza cookie-ul si token-ul cu 0 folosind functia memset.
- Intră într-o buclă infinită în care:
  - Citeste actiunea de la utilizator folosind functia fgets.
  - Daca actiunea este "exit", iese din bucla.
  - Altfel, deschide conexiunea cu serverul si executa actiunea corespunzatoare.
    Daca actiunea nu este recunoscuta, afiseaza un mesaj de eroare.
  - Inchide conexiunea cu serverul folosind functia close_connection.
- La sfarsit, elibereaza memoria alocata pentru actiune, cookie si token.
