# Manuel utilisateur

## Introduction
Un programme Arduino est structuré en 3 parties :
* les déclarations, inclusions, etc.
* la programme "setup", exécuté une seule fois
* le programme principal, exécuté en boucle.

Le menu permettant de configurer la balance est situé dans l'étape "setup" et n'est donc accessible qu'à la mise sous tension de la balance. Une fois qu'on a quitté le menu, on passe dans le programme principal.


## Le clavier
![clavier](/Images/CGScale-keyboard-fr.jpg)

## Configuration initiale

### Menu principal
Au démarrage de la balance, le menu suivant s'affiche : </br>
**`Configuration`** </br>
` Choix support` </br>
` Calibration` </br>
`>Quitter` </br>

Dans ce premier niveau de menu, la touche `Entrée` permet de sélectionner une action. `Quitter` va lancer le programme principale de la balance.
Dans les sous-menu, la sélection d'une option se fait avec la touche `Droite`, la touche `Gauche` permettant elle de revenir au menu principal.

### Menu Choix support
**`Support : petit `** </br>
` Petit` </br>
` Moyen` </br>
`>Grand` </br>

Ce sous-menu permet de choisir le type de support. Les supports sont montrés en photo dans cette page. Il est important de choisir le bon support car les caractéristiques sont différentes. Le support configuré dans la mémoire de la balance est affiché lors du démarrage.

### Menu Calibration
**`Calibration`** </br>
`>Config. tare` </br>
` Calib. avant` </br>
` Calib. arriere` </br>

Ce sous-menu permet 3 choses :
* définir le poids de référence de la tare que vous allez utiliser pour calibrer les balances
* calibrer la balance avant
* calibrer la balance arrière </br>

Chaque option est sélectionnée avec la touche `Droite`.

#### Définir le poids de la tare
L'écran affiche alors le dernier poids défini (il est conservé dans la mémoire permanente de la balance). Pour le modifier, on utilise les touches suivantes :
* `Gauche` et `Droite` pour changer la valeur de l'incrément
* `+` et `-` pour augmenter ou diminuer le poids de la valeur de l'incrément
* `Entrée` pendant 5s pour enregistrer le poids dans la mémoire de la balance, et revenir au sous-menu précédent.

#### Calibrer la balance avant
L'opération est très simple :
* poser la tare sur le support avant
* appuyer 5s sur la touche `Entrée` pour démarrer la calibration </br>

L'opération prend quelques secondes, voire une minute, mais pas au-delà. Si ça dure trop longtemps, c'est qu'il y a un problème, et le mieux est de redémarrer la balance.

#### Calibrer la balance arrière
Même opération que pour la balance avant. Seul différence, réussir à faire tenir la tare en équilibre sur les plots, l'exercice peut demander une certaine adresse et causer quelques énervements passagers.
Retour au menu principal avec la touche `Gauche`.

## Utilisation de la balance

Une fois calibrée, la balance fonctionne toute seule, posez un modèle sur les supports, et la lecture du poids et du CG se font en quelques secondes.

**Recommandations pour un bon fonctionnement**
* poser la balance sur une surface stable et plane
* ne pas toucher les supports de la balance pendant sa phase de démarrage
* éviter de peser un modèle lorsqu'il y a du vent, la lecture ne sera pas stable, car le modèle bouge.

## Problèmes rencontrés

### Stabilité des valeurs renvoyées par une balance
* les jauges de contraintes sont livrées avec des fils assez longs, qu'il faut raccourcir au maximum pour diminuer le "bruit", ce "bruit" va empêcher les amplificateurs HX711 de "lire" des valeurs stables et la calibration ne fonctionnera pas.
* pour atténuer encore plus le "bruit", il faut torsader les fils des jauges de contrainte.

### Lors de l'utilisation
* il peut arriver parfois qu'au démarrage, le programme "fige". Je ne sais pas pourquoi, à la compilation, tout est parfait, aucune alerte, la mémoire disponible est amplement suffisante. Je mets ça sur le compte des composants électroniques. Mais ça n'est pas très grave, il suffit de redémarrer la balance, et ça repart.




