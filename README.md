# NavesUSFX_022026 — Laboratorio 01

**Materia:** Programación Avanzada (SIS457)
**Docente:** Ing. Carlos Walter Pacheco Lora
**Motor:** Unreal Engine 4.27 — proyecto 3D en C++

---

## 1. Qué pedía el laboratorio

> Crear un proyecto en Unreal Engine, de tipo 3D, para construir el juego de Naves
> (nombre del proyecto `NavesUSFX_022026`). Crear una clase `Enemigo`, y hacer que
> esta se mueva por diferentes partes del escenario. Los objetos generados a partir
> de esta clase deben ser colocados en la escena por código C++ en el evento begin
> del GameMode.

## 2. Qué se desarrolló

### 2.1 La clase `Enemigo`

Archivos: [`Source/NavesUSFX_022026/Enemigo.h`](Source/NavesUSFX_022026/Enemigo.h) y
[`Enemigo.cpp`](Source/NavesUSFX_022026/Enemigo.cpp)

`AEnemigo` hereda de `AActor` y representa una nave enemiga. Está formada por:

| Elemento | Para qué sirve |
|---|---|
| `USceneComponent* Raiz` | Raíz del actor |
| `UStaticMeshComponent* Malla` | El cuerpo visible de la nave (un cono que hace de fuselaje) |
| `FVector Destino` | El punto del escenario hacia el que se dirige en este momento |
| Zona de patrullaje | El volumen del escenario dentro del cual puede moverse |

**Por qué la raíz es un componente vacío y no la malla:** el cabeceo del vuelo se
aplica moviendo la malla en coordenadas locales. Si la malla fuera el `RootComponent`,
ese movimiento local sobrescribiría la posición del actor en el mundo y la nave se
quedaría clavada en el origen oscilando, sin avanzar nunca hacia su destino. Separando
la raíz de la malla, cada cosa mueve lo suyo: el actor se traslada por el escenario y
la malla solo cabecea dentro de él.

### 2.2 Cómo se mueve por diferentes partes del escenario

El movimiento **no es una ruta fija**. Funciona en un ciclo de dos pasos:

1. **`ElegirNuevoDestino()`** — sortea al azar un punto dentro de la zona de
   patrullaje: una coordenada X, una Y, y una altura Z entre el mínimo y el máximo
   de vuelo. Así el enemigo puede terminar en cualquier parte del escenario.

2. **`MoverHaciaDestino()`** — se ejecuta en cada `Tick`:
   - avanza en línea recta hacia el destino a `Velocidad` cm/s
   - gira progresivamente el morro hacia el rumbo con `FMath::RInterpTo`, para que
     el cambio de dirección se vea natural y no como un salto
   - cuando queda a menos de `DistanciaDeLlegada` del punto, **elige otro destino**
     y vuelve a empezar

El resultado es que cada nave recorre el escenario de forma continua y distinta a
las demás, sin repetir siempre el mismo recorrido.

Además, la malla hace un leve cabeceo vertical (`FMath::Sin` sobre el tiempo de
vida) para que el vuelo no se vea rígido.

### 2.3 Colocación por código en el `BeginPlay` del GameMode

Archivo: [`NavesUSFX_022026GameModeBase.cpp`](Source/NavesUSFX_022026/NavesUSFX_022026GameModeBase.cpp)

Este es el requisito central del laboratorio. **Ningún enemigo se arrastra a mano en
el editor**: todos nacen desde código.

```cpp
void ANavesUSFX_022026GameModeBase::BeginPlay()
{
    Super::BeginPlay();

    PrepararEscenario();
    ColocarEnemigosEnEscena();   // <-- aquí se crean los enemigos
    ...
}
```

`ColocarEnemigosEnEscena()` recorre un bucle y por cada iteración:

- calcula una posición inicial repartida en círculo, para que las naves no nazcan
  una encima de otra
- le asigna un color distinto de una paleta, para poder distinguirlas
- llama a `CrearEnemigo()`, que hace el `SpawnActor` propiamente dicho

Se usa **`SpawnActorDeferred`** en lugar del `SpawnActor` normal. El motivo es
concreto: hay que configurar la zona de patrullaje y el color **antes** de que corra
el `BeginPlay` del enemigo, porque es ahí donde elige su primer destino y aplica su
material. Con un `SpawnActor` común el `BeginPlay` se ejecuta de inmediato y la
configuración llegaría tarde.

```cpp
AEnemigo* Nuevo = Mundo->SpawnActorDeferred<AEnemigo>(AEnemigo::StaticClass(), Transformacion);
Nuevo->DefinirZonaDePatrullaje(...);
Nuevo->EstablecerColor(Color);
UGameplayStatics::FinishSpawningActor(Nuevo, Transformacion);
```

### 2.4 El escenario también se genera por código

Para que el proyecto funcione al instante, sin tener que configurar nada a mano en
el editor, el GameMode también crea:

- **el piso** — un plano escalado al tamaño del escenario
- **la cámara** — ubicada de modo que encuadre toda la zona de patrullaje, y fijada
  como vista del jugador

## 3. Cómo ejecutarlo

1. Abrir `NavesUSFX_022026.uproject` con Unreal Engine 4.27
2. Presionar **Play**

No hay que configurar nada más: el GameMode ya está fijado en
`Config/DefaultEngine.ini`.

Al dar Play aparecen **8 naves de distintos colores** volando por el escenario, cada
una hacia su propio destino, cambiando de rumbo al llegar.

## 4. Parámetros ajustables

Todos son `UPROPERTY(EditAnywhere)`, así que se pueden modificar desde el editor sin
recompilar:

| Parámetro | Valor por defecto | Qué controla |
|---|---|---|
| `CantidadDeEnemigos` | 8 | Cuántas naves se colocan |
| `AnchoDelEscenario` / `LargoDelEscenario` | 6000 uu | Tamaño de la zona de patrullaje |
| `AlturaMinimaDeVuelo` / `AlturaMaximaDeVuelo` | 200 / 1100 uu | Rango de altura |
| `Velocidad` | 550 cm/s | Rapidez de avance de cada nave |
| `VelocidadDeGiro` | 3.0 | Qué tan rápido endereza el rumbo |
| `DistanciaDeLlegada` | 150 uu | Cuándo se considera que llegó al destino |

## 5. Estructura del proyecto

```
NavesUSFX_022026/
├── Config/
│   ├── DefaultEngine.ini              GameMode y mapa por defecto
│   ├── DefaultGame.ini
│   └── DefaultEditor.ini
├── Source/
│   ├── NavesUSFX_022026.Target.cs
│   ├── NavesUSFX_022026Editor.Target.cs
│   └── NavesUSFX_022026/
│       ├── NavesUSFX_022026.Build.cs
│       ├── NavesUSFX_022026.h / .cpp           módulo principal
│       ├── Enemigo.h / .cpp                    LA CLASE ENEMIGO
│       └── NavesUSFX_022026GameModeBase.h/.cpp COLOCACIÓN POR CÓDIGO
└── NavesUSFX_022026.uproject
```

## 6. Video explicativo

*(pendiente de agregar el enlace)*
