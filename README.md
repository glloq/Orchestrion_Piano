> [!NOTE]
> travail en cours et je n'ai pas la place pour tester !

# Presentation 

l'objectif est d'avoir un code qui est facilement adaptable pour differents piano.
Nous utiliserons des pac9685 pour controller la puissance de frappe de chaque solenoide.

# montage electrique

le montage est relativement simple, il ne faudra qu'un arduino, des pca 99685, des mofset de puissance et des solenoides (avec l'alimentation adapté) 
![Schema electrique](https://raw.githubusercontent.com/glloq/Orchestrion_Piano/main/img/branchement%20pwm.png?raw=true)

des mofset serait adapté a cette utilisation mais il faut pensser a l'ajout d'une diode de roue libre !
Selectionner des solenoides qui n'ont pas un amperage trop elevée pour limiter la surchauffe => il est possible de dissiper le chaleur en fixant les solenoides sur une plaque aluminium

> [!TIP]
> pour les solenoides, viser une alimentation en 24v, un deplacement de 10mm avec 5 a 7N de force.

l'idée est de fixer les soilenoides sous les touches et d'actionner directement les touche via une tige metalique.
![Schema mecanique](https://raw.githubusercontent.com/glloq/Orchestrion_Piano/main/img/montage.png?raw=true)

> [!CAUTION]
> penser a l'ajout de fusibles ou disjoncteurs pour les parties 230v et 24v!


# Settings

Il n'y a que peu de reglages a prendre en compte dans ce projet :
- le numero midi de la premiere note (la plus grave) 
- le pwm minimum pour activer le solenoide
- le nombre de note jouable
- les pins de sorties pour les pedales

  
