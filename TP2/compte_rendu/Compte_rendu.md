# Barret Alexandre - Mizoules Vincent
# Compte rendu - TP2

## Usage

Pour lancer notre programme et afficher l'image résultante, on utilise les commandes suivantes :

	make && ./tp2

Le facteur de qualite est demandé.

Pour nettoyer le projet

	make clean
	make clear

## Réponses aux questions de l'exercice 1 

### 1)

L'algorithme d'encodage est réalisé lors du make (fonction JPEGEncoder)

### 2)

L'algorithme de décodage est réalisé lors du make (fonction JPEGDecoder)

### 3)

Le facteur de qualité est demandé lors de l'éxecution du programme, afin d'observer les effets lors de l'évolution de ce dernier.
Nous lançons tout d'abord le programme en donnant un facteur de qualité de 1, ensuite nous iréiterons l'opération avec 20. 

L'image est semblable mais est beaucoup moins net. On voit des blocs de 8px*8px qui apparaissent.


Au final, nous obtenons le processus de compression et décompression JPEG suivant :

Photo originale > Découpage en blocs (8 x 8 pixels) > DCT > Quantification > Quantification inverse > DCT inverse >  Recomposition > Affichage des images
