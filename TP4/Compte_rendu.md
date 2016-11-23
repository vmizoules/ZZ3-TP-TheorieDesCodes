# Barret Alexandre - Mizoules Vincent
# Compte rendu - TP4

## Usage

Pour lancer notre programme :

	make && ./tp4

Pour nettoyer le projet

	make clean
	make clear

Il est possible de désactiver l'affichage des informations de debug en commentant/décommantant les `define` présents au début du programme.

## Réponses aux questions

### 1)

La fonction nommé "GSM_decode" fonctionne sur le principe suivant :

	Tout d'abord on crée l'état initial 0000 (avec input "", dist 0, diff -1) et on l'ajoute à la liste.
	Pour chaque groupe de 2 bits du message 
	|	Pour chaque état existant
	|	|	S'il n'est pas nouveau alors 
	|	|	|	On calcule l'état suivant avec une entrée valant 0
	|	|	|	On calcule l'état suivant avec une entrée valant 1
	|	|	finSi
	|	FinPour
	FinPour 
	On récupère la chaine "input" de notre état racine, puisque l'algorithme converge.


La fonction permettant de calculer l'état suivant fonctionne sur le principe suivant :

	Calcul le nom du futur état 
	Si le futur état existe
	|	Récupere l'état existant
	|	Si sa différence vaut -1
	|	|	// l'état n'a pas été modifié lors de ce tour de boucle)
	|	|	On calcule la différence et on la met à jour dans l'état
	|	|	On met à jour l'input de l'état
	|	Sinon
	|	|	On calcule la nouvelle et l'ancienne distance totale
	|	|	Si l'ancienne est plus petite que la nouvelle
	|	|	|	On laisse l'ancien état tel quel
	|	|	Sinon
	|	|	|	On met à jour l'input de l'état
	|	|	|	On recopie l'ancienne distance dans cet état
	|	|	|	On calcule la différence et on la met à jour dans l'état
	|	|	FinSi
	|	FinSi
	Sinon
	|	// création d'un nouvel état
	|	On calcule la différence
	|	On crée un nouvel état
	|	On met à jour l'input
	|	On l'insère dans la liste
	FinSi


La fonction de décodage fonctionne parfaitement. Le message original est retrouvé dans l'état racine avec une distance de 0 (puisqu'il n'y a pas eu de modification).

### 2)

La fonction permettant d'ajouter des erreurs dans le message utilise une variable "ErrorRate".
Elle ajoute donc des erreurs en fonction du taux d'erreur défini.


Après quelques testes on voit qu'on retrouve le message original même avec 4 erreurs sur le canal (avec NbMot=20)


