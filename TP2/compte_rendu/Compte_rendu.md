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


Le but de ce TP est de réaliser un processus de compression et décompression JPEG tel que suit :


Photo originale > Découpage en blocs (8 x 8 pixels) > DCT > Quantification > Quantification inverse > DCT inverse >  Recomposition > Affichage des images


### 1)

L'encodage est réalisé dans la fonction JPEGEncoder et suit l'algorithme suivant :

	Découpage en blocs (8 x 8 pixels)
	Pour chaque bloc
		DCT sur le bloc
		Quantification sur le bloc
		Réinsersion du bloc dans l'image
	FinPour
	Retourne l'image compressée


### 2)

Le décodage est réalisé dans la fonction JPEGDecoder et suit l'algorithme suivant :

	Découpage en blocs (8 x 8 pixels)
	Pour chaque bloc
		Quantification inverse sur le bloc
		DCT inverse sur le bloc
		Réinsersion du bloc dans l'image
	FinPour
	Retourne l'image décompressée


### 3)

Le facteur de qualité est demandé lors de l'éxecution du programme, afin d'observer les effets lors de l'évolution de ce dernier.
Pour les tests qualitatifs, nous lançons le programme en donnant un facteur de qualité de 1, ensuite nous iréiterons l'opération avec de nouvelles valeurs (voir captures "Quality*.png"). 
Plus le facteur qualité est important plus l'image est floue et on voit des blocs de 8px*8px qui apparaissent.

Pour les tests quantitatifs, nous utilisons la méthode MSE qui calcule le taux de distorsion entre les deux images (avant compression et après compression), et nous notons les résultats dans le fichier ("DistorsionQuality-Chart").
Nous obtenons ensuite une courbe représentative de l'évolution de la distorsion en fonction du facteur de qualité entré (voir fichier "DistorsionQuality-Chart.pdf").
