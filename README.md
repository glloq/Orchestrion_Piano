# Orchestrion_Piano
a piano midi usb controlled that play an accoustic piano

# Presentation 

l'objectif est d'avoir un code qui est facilement adaptable pour differents piano.
Nous utiliserons de pac9685 pour controller la puissance de frappe de chaque solenoide.



# montage electrique

le montage est relativement simple, il ne faudra qu'un arduino, des pca 99685, des mofset de puissance et des solenoides (avec l'alimentation adapté) 

# Settings

Il n'y a que peu de reglages a prendre en compte dans ce projet :
- le numero midi de la premiere note (la plus grave) 
- le pwm minimum pour activer le solenoide
- le nombre de note jouable
- les pins de sorties pour les pedales
