# RayCaster 
Afin d'avoir un rendu 3D pour ce projet je me suis aider de plusieurs ressources car j'avais du mal à visualiser le côté mathématique du problème
J'ai commencé par regarder [cette vidéo](https://www.youtube.com/watch?v=eOCQfxRQ2pY) qui m'a permis de comprendre la façon d'utiliser les principes mathématique. J'ai ensuite regardé [cette autre vidéo](https://www.youtube.com/watch?v=gYRrGTC7GtA) qui m'a permis de comprendre comment avoir le rendu sans utiliser les fonctions qui était déjà fournies avec ce projet et ensuite [ce site](https://lodev.org/cgtutor/raycasting.html) m'a permis de comprendre et d'utiliser les fonctions fournies

## Optimisation
Une façon d'optimiser le raycaster serait d'utiliser notre GPU pour colorier les pixels de notre écran plutôt que de le faire pixel par pixel dans le code. 
L'utilisation de VerticesArray nous permettrait en peu d'instructions de colorier des triangles qui composent notre rendu
