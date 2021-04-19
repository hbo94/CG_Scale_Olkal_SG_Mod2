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
Ce sous-menu permet de choisir le type de support. Pour l'instant, les trois supports disponibles ont tous les trois les mêmes caractéristiques, donc il n'est pas utile de changer le type de support. Cela sera utile dans une prochaine version du logiciel, avec de plus grands supports.

### Menu Calibration
Ce sous-menu permet 3 choses :
* définir le poids de référence de la tare que vous allez utiliser pour calibrer les balances
* calibrer la balance avant
* calibrer la balance arrière



