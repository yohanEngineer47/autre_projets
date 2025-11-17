# TP du jeu de la roulette dans le casino
from random import randrange
from math import ceil
from os import system

# les variables
argent_initial = 1000 # 1000 $ au début du jeu
continuer_la_roulette = True # tant que tu continue la partie

print('vous entrer dans le jeu avec',argent_initial,'$')
while continuer_la_roulette:
    # vérifier si l'utilisateur a répondue au condition sur le nombre miser ainsi que la somme
    nombre_P = 50 # aide de la corection clairement
    while nombre_P > 49 or nombre_P < 0:
            nombre_P = input('choisie entre 0 et 49 :')
            try:
                nombre_P = int(nombre_P)
            except ValueError:
                print('saisiessez correctement !!!')
                nombre_P = 50
                continue
            if nombre_P > 49:
                print('tu as choisie un nombre supérieur a 49')
            if  nombre_P < 0:
                print('ton nombre est négative ')
    # vérifier si le joueur a répondue correctement a la question sur sa mise
    mise_initial = 0
    while (mise_initial == 0 or mise_initial <= 0) or mise_initial > argent_initial:
        mise_initial = input('met ta mise:')
        try:
            mise_initial = int(mise_initial)
        except ValueError:
            print("hein !!!??? j'ai rien compris")
            mise_initial = 0
            continue
        if mise_initial > argent_initial:
            print ("c'est + ce que tu as")
        elif mise_initial <= 0:
            print('mdr')
    # la roulette est lancé
    nombre_gagnant = randrange(50)
    print('le nombre gagnant est le',nombre_gagnant)
    if nombre_P == nombre_gagnant:
        mise_gagnant = mise_initial*3
        print('wooooahaah tu as gagné', mise_gagnant,'$')
        argent_initial += mise_gagnant
    if nombre_P != nombre_gagnant:
        autre_somme = ceil(mise_initial*0.5) # autre_somme = mise_initial*(50/100) # au cas oú si la somme est paire ou impaire
        if (nombre_P % 2 == 0) and (nombre_gagnant % 2 == 0):
            print('votre nombre proposer est noire comme le nombre gagnant donc vous gagné', autre_somme,'$')
            argent_initial += autre_somme
        elif (nombre_P % 2 != 0) and (nombre_gagnant % 2 != 0):
            print('votre nombre proposer est rouge comme le nombre gagnant donc vous gagné', autre_somme,'$')
            argent_initial += autre_somme
        else:
            print('désolé tu as perdue')
            argent_initial -= mise_initial
            # demande a l'utilisateur si il veut toujours continuer la partie
    print('vous avez a présent', argent_initial,'$')# on affiche sa somme actuel
    # Si tu as complétement perdue bref sans un sous
    if argent_initial <= 0:
        print("vous n'avez plus d'argent, ...Game over")
        continuer_la_roulette = False
    else :
        y = True
        while y:# mettre une condition pour pouvoir faire tourner la boucle peut importe le contexte de cette boucle tant
        # qu'il a de l'argent on lui pose la question de si il veut toujours continuer la partie ou pas
            le_choix = input('voulez-vous quitter (o/n) ?')
            try:
                le_choix == 'o' or le_choix == 'O' or le_choix == 'n' or le_choix == 'N'
            except:
                print ('...???')
                continue
            z = True
            while z:
                if le_choix == 'o' or le_choix == 'O': # si jamais il accepte
                    print ('vous reparter avec,',argent_initial,'$')
                    z = False
                    y = False
                    continuer_la_roulette = False # fin de la partie
                elif le_choix == 'n' or le_choix == 'N':# le joueur veut continuer il peut continuer
                    print ('ok, continuons !!!')
                    y = False
                    z = False
                else: # on a pas compris de ce que vous avez dit on va repeter la question
                    z = False

# pause de la commande (windows)
system('pause')
