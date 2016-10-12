# Barret Alexandre - Mizoules Vincent
# Compte rendu - TP2

## Usage

Pour lancer notre programme et afficher le graph résultant, on utilise les commandes suivantes :

	make && ./tp2

Le facteur de qualite est demandé.

Pour nettoyer le projet

	make clean
	make clear

## Reponses aux questions de l'Exercice 1 

### 1

L'algorithme d'encodage est réalisé lors du make (fonction JPEGENCODER)

### 2

L'algorithme de décodage est réalisé lors du make (fonction JPEGDecoder)

### 3

Afin d'observer l'evolution du facteur de qualité, celui-ci est demandé lors de l'execution du programme. 

Au finale, nous obtenons le processus de compression et decompression JPEG suivant :

Photo originale > Découpage en blocs (8 x 8 pixels) > DCT > Quantification > Quantification inverse > DCT inverse >  Recomposition > Display Picture
