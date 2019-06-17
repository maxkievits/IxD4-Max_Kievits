# IxD4-Max_Kievits
Kernmodule Interaction Design 4

Technische werking Prototype

Iteratie 1:
Ik heb 5 jumper wires in een plastic flesje geprikt. Op de onderste staat 5V, en de bovenste vier werken als Input. Zodra er water in komt, zal het water werken als geleider en kan ik meten of er stroom bij de inputs binnenkomt. Op die manier weet ik hoe hoog het water staat in het flesje. Maar... water blijkt in de praktijk toch niet zo'n goede geleider, dus moest ik wat anders verzinnen...

Iteratie 2:
Ik heb een grotere fles gepakt en aan de onderkant van de deksel een ultrasonic sensor geplaatst die naar beneden wijst. Deze sensor stuurt een audio signaal af en kan het vervolgens weer opvangen. Door de tijd te berekenen tussen het versturen en ontvangen, kan hij meten hoe ver hij van een object af staat. Mijn object is in dit geval water die in de fles zit. Deze gegevens worden vergeleken met de diepte van de fles en omgerekend tot een percentage. Dit percentage wordt samen met het tijdstip opgeslagen in de database.

In de app wordt de meest recente record van de database opgehaald en getoond aan de gebruiker. Per keer dat de inhoud veranderd (wanneer de gebruiker drinkt of het flesje bijvult), worden deze gegevens bij het dagtotaal opgeteld en wordt het dagelijks doel geüpdate. Alle verzamelde gegevens (fles-capaciteit en datum + tijd) worden gebruikt om op de achtergrond een slim profiel te genereren, die precies weet wanneer het tonen van een melding (herinnering om te drinken) het meest succesvol is.
