Readme - RGB LED Matris 64x 32 pixels:

0 ORIENTERING

1 HUR DET GICK TILL

2 Prestanda

3 Tester

4 EKONOMI, BOM - lista/ kostnader

5 BILAGOR

0 ORIENTERING:

Tanken bakom projektet att byta display på ett prototypbygge av en förförstärkare med elektronrör var att innom ramen för inlämningsuppgiften specialarbete i kursen programmering och utveckling av inbyggda syetem byta ut en LCD display mot en LED display. Den gamla LCD:n var rerlativt ljusstark, men hade dålig kontrast och betrakningsvinkel. Det gjorde den svår att läsa på håll och från sidan. En annan faktor som gjorde bytet intressant var ökad teckenhöjd och lägre pris.

1 HUR DET GICK TILL:

RGB Ledmatrisen och det ”Grafikkort” som framtagits i specialarbetet under kursen ”Programmering och utveckling av inbyggda system” är den tredje ”generationen” av displayer använda i mitt projekt ”Ett prototypbygge av en elektronrörförförstärkare”, som finns beskrivet på Linkedin: https://www.linkedin.com/feed/update/urn:li:activity:7050475724884828160/.

Första displayen jag använde visade sig vara för långsam. All grafik ”skyfflades” ut från förstärkarens huvud mikrokontroller via det I2C gränssnitt som styr hela förstärkaren. Och att uppdatera den nedersta raden av fyra som anger tiden, tog lite knappt 1/2 sekunder och en total uppdatering ca 1 1/2 sekunder.

För att råda bot på problemet uppdaterade jag förstärkaren till en modernare display, där fonter och grafik kunde sparas i displayens minne. Nu svarade allt betydligt fortare och jag kunde känna mig till freds.

Förstärkaren hamnade nu i vardagsrummet, för utprovning. En av de första sakerna jag upptäckte var att den nya displayen, som på nära håll var skarp, visade sig dels vara svårläst på håll och för att göra det extra tråkigt så hade den, som många andra LCD displayer, dålig betraktningsvinkel. Det var helt enkelt omöjligt att läsa displayen.

Så en dag någon gång under våren/ försommaren såg jag den på Amazon. - En trefärgs LED Matris på 64x 32 pixlar. Priset var 1/4 till 1/3 av det för den tidigare LCD:n. Till detta kommer dock att det behövs en mikrokontroller för att skapa interface mellan förstärkarens centrala mikrokontroller och den nya LED matrisen. Behövs en mikrokontroller så behövs även ett kretskort där den och andra kringkomponenter ingår. När det blev klart att det skulle bli ett specialarbete, så kände jag att det skulle kunna vara intressant att implementera en LED matris i förstärkaren.

Så när sommaren kom skred jag till verket. Det hamnade två mikrokontrollerkretsar av typen PIC18F47K42 på en bread board. En som skickar SPI meddelanden och en som tog emot dessa och la upp meddelandena på LED matrisen. Det visade sig mycket snart att mina försök gav flimriga bilder, efter som uppdateringen var långsam och dessutom fanns fenomenet att den gav återkommande ljus flashar.

För att åtgärda flimret ökade jag processor klockan från 16 MHz till 64 MHz. – Nu skulle det bli bra! -Tji. Displayen uppdaterade inte över huvud taget. Vid mätningar med oscilloskop kunde jag konstatera att vissa portutgångar på mikrokontrollern inte gav de resultat jag förväntat mig, Hmm, konstigt. Efter några dagars febrilt sökande efter lösningar skapade jag ett ärende hos mikrokontrollertillverkaren, Microchip, och efter ca två dagar föreslog deras support att jag skulle skriva datat direkt till kretsens utgångs-latchar, snarare än rakt på portarna. Så jag provade och bilden blev stabil.

Kvar fanns dock ljusflasharna. Efter en del funderande insåg jag att dessa förmodligen berodde på att SPI rutinen. Om rutinen, som ligger i ett interrupt, avbryter uppdateringen av displayen vid ett tillfälle, där displayen är satt att visa senaste skrivna datat. Och hålls i detta tillstånd medan interruptet gör sitt, en kort, men märkbar förlängning av displaytiden. Åtgärden blev att inleda interruptet med att att ”släcka” dispayen genom att sätta ~ENA biten.

Nu fungerade displayen nästan som förväntat. Sommaren tog slut och jag gjorde Masken spelet klart och sedan bar det av till Biltema för att köpa akrylglas. Jag monterade sedan komponenterna på ett mönsterkort som jag beställt från Kina, sågade och borrade akrylglas. För att ge displayen lite bättre finish, så skrev jag ut en laserutskrift med texter för vad de lysdioder, mfl funktioner, som utöver LED matrisen, finns på Kretskortet. Utskriften laminerades och sedan skar jag ut de fyrkantiga hålen med skalpell och de runda hålen klippte jag med en håltång.

Vid det här laget var det dags att åka till Trollhättan och displayen fick följa med. Det kändes som om den blev väl mottagen och allt var bra.

Vid hemkomsten efter Trollhättan började jag ”re fakturera” koden i förstärkaren och efter några dagars jobb kom det meddelanden från förstärkarens centrala mikrokontroller, skönt... Så började jag ”gläppa” lite med fjärrkontrollen – och så började jag se ”fenomenen”. Min normalt vita displaytext blev under korta stunder gul, blå och alla de färger displayen kan generera. F**n – inte nu i gen – jag som trodde det var klart.

Det gick någon dag och en väldig massa gläppande på fjärrkontrollen – så dök uppenbarelsen upp. -Jag får inte avbryta en ”display frame” under uppdatering, för att sedan fortsätta med något helt annat. Jag var helt enkelt tvungen att se om jag hade läst meddelandet färdigt, innan jag lägger ut det på displayen.

Jag bytte mitt 0x1B (ESCAPE) för att inleda ett meddelande, till 0x02 (START OF TEXT) och satte ett 0x03 (END OF TEXT) som slut på meddelandet. Om meddelandet slutar med 0x03 så noteras det av en if-sats och displayen kan uppdateras.

På PCB:n finns även några lysdioder för att visa statusen för den bluetooth modul som byggts in i förstärkaren, en IR-mottagare för fjärrkontroll och en fotodiod. Den senaste mäter omgivningsljuset och styr displayens ljusstyrka. Efter en del dribblande med datatyper och formler så kommer ett heltalsvärde ut. Värdet matas in i en timer som styr hur länge displayen ska visa ut värdet på lysdioderna via ~ENA funktionen.

2 Prestanda:

Prestandan är överlägsen både i läsbarhet och snabbhet. Upplösningen är dock bara 1/15 mätt i monokroma pixlar, eller 1/5 mätt i totala mängden aktiva pixelelement Samtidigt så ger den låga upplösningen en intressant look

Vad gäller energiförbrukning så är det svårt att skatta. Det som går att säga är att förstärkarens elektronrör behöver ca 30 W för glödström. Jag tänker mig att det är mer än displayen. Samtidigt så är förmodligen displayen den stora förbrukaren i stand by läge. Förstärkaren går att stänga av, men det ger problem med klock/ kalenderkretsen som går några sekunder fort varje dygn. Detta är kompenserat för genom att klock/ kalender kretsen justeras av den centrala mikrokontrollern en gång per dygn och det kan bara ske om förstärkaren är i stand by eller drift.

3 TESTER

Fem felfall har skapats för att se hur displayen reagerar på felaktiga data.

3.1 Dubbel start kod, ASCII 0x02, Start of text. Om displayen klarar felet kommer texterna "TEXT 1", "TEXT 2", "TEXT 3" och "TEXT 4" att skrivas på respektive rad. Det innebär att displayens firmware reder ut att starta direkt efrer en andra initieringen med Start of text.

3.2 Dubbelt stopp, ASCII 0x03, End of text. Om displayen klarar felet kommer texterna "TEXT 1", "TEXT 2", "TEXT 3" och "TEXT 4" att skrivas på respektive rad. Det innebär att displayens firmware reder ut att starta direkt efrer endubbel avslutning med End of text.

3.3 End of text, ASCII 0x03, skrivs tvärt in i text överföringen. Om testet går "rätt" komer inte raderna enligt 3.1 och 3.2 att visas. I stället kommer en förklaring varför inte att visas på displayen.

3.4 Start of text, ASCII 0x02, skrivs tvärt in i textöverföringen, men kompletteras inte av full textmassa. Om testet går "rätt" komer inte raderna enligt 3.1 och 3.2 att visas. I stället kommer en förklaring varför inte att visas på displayen.

3.5 Start of text, ASCII 0x02, skrivs tvärt in i textöverföringen och kompletteras av full textmassa. Texterna enligt 3.1 och 3.2 kommer att visas på respektive displayrad. Testet visar att display och firmware klarar att direkt fånga upp ett nytt meddelande.


4 EKONOMI, BOM - lista/ kostnader:

Kostnaden som finns angiven i Bilaga 6 avser styckpriset för framtagandet av 5 enheter. Kostnad ~700/ st Den gamla lösningen där en komplett LCD enhet kostar ~1500/ st Se bilaga 6 för detaljerade siffror.

5 BILAGOR:

Bilaga 1: Grafikkort Kretsschema.pdf PDF:en innehåller komplett koplingsschema för "Grafikkortet" 

Bilaga 2: Grafikkort Mönsterkort Front Side.pdf PDF:en innehåller Mönsterkortets framsida 

Bilaga 3: Grafikkort Mönsterkort Rear Side PDF:en innehåller Mönsterkortets baksida 

Bilaga 4: Grafikkort 3D VY Framsida PDF:en innehåller en 3D Vy över kortets framsida 

Bilaga 5: Grafikkort 3D VY Baksida PDF:en innehåller en 3D Vy över kortets baksida 

Bilaga 6: Bilaga 6 BOM Display PDF:en innehåller en materiellista med de faktiska kostnader som finns,
          undantaget kolumnen med artiklar som redan fanns hema, som är uppskattade priser

Bilaga 7: YouTube - Video Inbyggda System
