# Quick Time

## Descriere

In aceasta tema, se va crea un joc de reflex competitiv pentru doi jucatori, in care cei doi participanti vor concura pentru a obtine cel mai mare punctaj, testandu-si viteza de reactie. Fiecare jucator va avea butoane si LED-uri proprii, iar jocul se va desfasura in mai multe runde. Scopul fiecarui jucator este sa apese cat mai rapid butonul corespunzator culorii afisate pe LED-ul RGB al echipei sale. Punctajul fiecarui jucator va fi afisat pe un ecran LCD si se va actualiza pe parcursul jocului. La final, jucatorul cu cel mai mare punctaj va fi declarat castigator.

### Cerinte: 

**SPI**<br>
- Tema necesita utilizarea a doua placi Arduino Uno care comunica prin SPI, deoarece un singur Arduino nu ofera suficienti pini GPIO. Arduino-ul master controleaza LCD-ul, servomotorul si gestioneaza starea jocului (punctaj, LED-uri active). Arduino-ul slave controleaza butoanele si LED-urile, primind comenzi de la master si trimittand inapoi informatii despre butoanele apasate.

**Butoane**<br>
- Orice buton incepe jocul. In timpul jocului, butoanele trebuie folosite doar pentru controlul jocului si nu trebuie sa reseteze progresul. Doar butoanele jucatorului activ in acea runda pot controla jocul.

**LED-uri**<br>
- Fiecare buton are asociat un LED de o culoare diferita. Pe parcursul jocului acestea trebuie sa fie aprinse pentru a vedea carei culoare ii corespunde fiecare buton
- LED-ul rgb trebuie să se aprinda in una din cele 3 culori ale butoanelor
- LED-ul rgb trebuie sa fie stins daca nu este runda jucatorului corespunzator acelui LED

**LCD**<br>
- Pentru controlul acestuia am folosit biblioteca <LiquidCrystal>
- Vor fi folositi doar pinii D4-7 pentru liniile de date ale ecranului
- Pe parcursul jocului trebuie sa afiseze punctajele celor 2 jucatori

**Servomotor**<br>
- Servomotorul va incepe de la pozitia de 0 grade si se va deplasa in sens antiorar pentru a indica scurgerea timpului.

### Componente:

- **<b>6x LED-uri (2 grupuri de câte 3 leduri, în cadrul unui grup trebuie să avem culori diferite)<b>** 
- **<b>2x LED RGB (1 pentru fiecare jucător)<b>** 
- **<b>6x butoane (3 pentru fiecare jucător)<b>**
- **<b>2x Rezistoare 1kΩ<b>**
- **<b>1x LCD<b>**
- **<b>1x servomotor<b>**
- **<b>2x Breadboard<b>**
- **<b>Fire de legatura<b>**
- **<b>2x Arduino Uno<b>**

## Poze setup fizic

![quick-time](https://github.com/user-attachments/assets/920b41d1-b69e-466a-8230-6e03d6441481)

## Functionalitate montaj

https://github.com/user-attachments/assets/53b7c999-682d-4146-9162-69cc824be385

## Schema electrica

![schema-thinkercad](https://github.com/user-attachments/assets/15ceb907-4629-4bbc-bbb2-40efc1c4392e)

![schema-electrica](https://github.com/user-attachments/assets/27bee766-50d9-4251-b0f6-993c28f64503)



