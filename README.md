> [!NOTE]
> je fait ca pour le fun, je n'ai pas la place pour tester !

# Presentation 

l'objectif est d'avoir un code qui est facilement adaptable pour differents piano.
Nous utiliserons des pac9685 pour controller la puissance de frappe de chaque solenoide.
l'idée de base est d'utilsiser un cable usb pour envoyer les messages midi a un arduino qui viendra interpreter et exectuer les notes demandée sur le piano.

# montage electrique

> [!CAUTION]
> penser a l'ajout de fusibles ou disjoncteurs pour les parties 230v alternatif et la partie basse tension en courrant continu!

il faut adapter l'alimentation des solenoides en fonction de la consomation !
Si ont part du principe que l'on a un piano 88 touches et des solenoides de 0.5A, avec 88 solenoides il faut bien prevoir l'alimentation !
en prenant en compte le pic d'intensité à l'activation, il faut prevoir jusqu'a 3 a 5 fois la consomation pendant quelques ms=> donc 2.5A pour un seul soloenoide!!


il y a une methode pour limiter le nombre maximum de solenoides actif en meme temps sur une seule alimentation, il faut repartir plusieurs alimentations sur toute la surface des touches et alterner pour limiter a 5 le nomrbe maximum de solenoides actif en meme temps pour chaque alimentation ( par exemple 4 alimentation de 15A  alterné toutes les 2 touches) 

> [!TIP]
> prevoir l'ajout d'un filtre LC ou de condensateur proche des solenoides ?

## les touches

le montage est relativement simple, il ne faudra qu'un arduino, des pca 9685, des mofset de puissance et des solenoides (avec l'alimentation adapté) 
![Schema electrique](https://raw.githubusercontent.com/glloq/Orchestrion_Piano/main/img/branchement%20pwm.png?raw=true)

des mofset serait adapté a cette utilisation mais il faut pensser a l'ajout d'une diode de roue libre !
Selectionner des solenoides qui n'ont pas un amperage trop elevée pour limiter la surchauffe => il est possible de dissiper le chaleur en fixant les solenoides sur une plaque aluminium ou autre metal qui aide a une dissipation thermique passive !

> [!TIP]
> pour les solenoides, viser une alimentation en 24v, un deplacement de 10mm avec 5 a 7N de force.
> ce type de solenoide consome en moyenne 0.4 a 1A 

l'idée est de fixer les soilenoides sous les touches et d'actionner directement les touche via une tige metalique.
![Schema mecanique](https://raw.githubusercontent.com/glloq/Orchestrion_Piano/main/img/montage.png?raw=true)

> [!TIP]
> il faudra ajouter ou retirer du poid sur/dans la touche pour adapter la macanique au solenoide

> [!CAUTION]
> les vieux piano utilise du plomb !

## les pedales

On va avoir un probleme avec les pedales, il est necessaire d'avoir beacoup plus de couple que pour les touches => donc un amperage plus important et plus de chaleur generé
il est possible d'utiliser des solenoides mais il serait plus sage de partir sur un mecanisme a base de servomoteurs pour limiter les surchauffes et permettre un jeux plus complexe.

Il faut donc prevoir 3 servomoteurs et un mecanisme qui respecte 3 contraintes :
- fonctionne avec suffisament de couple et de precision 
- permet une activation rapide (moins de 0.5s max)
- permet un maintient de la position sans etre alimenté

Un systeme d'arbre a came pourrait fonctionner ?
  
# Utilisation du code

## Settings

Il n'y a que peu de reglages a prendre en compte dans ce projet :
- le numero midi de la premiere note (la plus grave) 
- le pwm minimum pour activer le solenoide
- le nombre de note jouable
- les pins de sorties pour les pedales
- les plage de position des pedales


  
