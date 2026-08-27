# Guion del video — Laboratorio 01 (máximo 3 minutos)

**Cómo grabar:** presiona `Win + G` → botón de grabar (Xbox Game Bar, ya viene en Windows 11).
El video queda en `Videos\Capturas`.

**Dónde subirlo:** YouTube, en modo **"No listado"** (lo ve cualquiera con el link, pero no
aparece en búsquedas). Ese link va al ecampus.

---

## 0:00 – 0:15 · Presentación

> "Buenas, soy [tu nombre]. Materia Programación Avanzada, SIS457. Voy a explicar el
> Laboratorio 01: el proyecto NavesUSFX_022026, hecho en Unreal Engine 4.27 con C++."

**En pantalla:** el proyecto abierto en Unreal, o la carpeta en el explorador.

---

## 0:15 – 0:45 · La clase Enemigo

**En pantalla:** Visual Studio con `Enemigo.h` abierto.

> "El laboratorio pedía crear una clase Enemigo. Acá está: `AEnemigo`, que hereda de
> `AActor`. Tiene una malla que es el cuerpo de la nave, y un `FVector Destino` que es
> el punto del escenario hacia donde se dirige en este momento."

Señalar con el mouse:
- la línea `class NAVESUSFX_022026_API AEnemigo : public AActor`
- el `UStaticMeshComponent* Malla`
- el `FVector Destino`

---

## 0:45 – 1:25 · Cómo se mueve por el escenario

**En pantalla:** `Enemigo.cpp`, bajar hasta `ElegirNuevoDestino()` y `MoverHaciaDestino()`.

> "El movimiento no es una ruta fija, funciona en dos pasos. Primero
> `ElegirNuevoDestino` sortea al azar un punto dentro de la zona de patrullaje: una X,
> una Y y una altura. Y después `MoverHaciaDestino`, que corre en cada Tick, avanza
> hacia ese punto y gira el morro progresivamente con `RInterpTo`."

> "Cuando la nave queda a menos de la distancia de llegada, elige otro destino y vuelve
> a empezar. Por eso recorre diferentes partes del escenario y no siempre el mismo
> camino."

---

## 1:25 – 2:00 · La colocación por código en el GameMode

**En pantalla:** `NavesUSFX_022026GameModeBase.cpp`, función `BeginPlay`.

> "Este es el punto central del laboratorio: los enemigos se colocan en la escena por
> código C++, en el evento begin del GameMode. Ninguna nave está arrastrada a mano en
> el editor."

Señalar el comentario grande y la llamada a `ColocarEnemigosEnEscena()`.

> "Ese método recorre un bucle, calcula una posición inicial repartida en círculo para
> que no nazcan encimadas, le da un color distinto a cada una, y hace el spawn."

Bajar a `CrearEnemigo()`:

> "Uso `SpawnActorDeferred` en vez del spawn normal, porque necesito configurar la zona
> de patrullaje y el color **antes** de que corra el BeginPlay del enemigo — que es
> donde elige su primer destino. Con el spawn común llegaría tarde."

---

## 2:00 – 2:45 · Demostración funcionando

**En pantalla:** Unreal Engine, presionar **▶ Play**.

> "Al dar Play, el GameMode arma el piso, coloca las ocho naves y ubica la cámara. Todo
> por código."

Dejar correr y narrar:

> "Cada nave tiene su propio destino, por eso van cada una por su lado. Vean cómo giran
> suavemente cuando cambian de rumbo — eso es el `RInterpTo`. Y el cabeceo que hacen es
> para que el vuelo no se vea rígido."

Presionar **Esc** para salir.

---

## 2:45 – 3:00 · Cierre

> "Todo el código está en el repositorio de GitHub, el link está en la entrega del
> ecampus. Gracias."

**En pantalla:** la página del repositorio en GitHub.

---

## Checklist antes de grabar

- [ ] Abrir Unreal y esperar a que termine de cargar (que no salga el diálogo de compilar)
- [ ] Abrir Visual Studio con los archivos `Enemigo.h`, `Enemigo.cpp` y el GameMode
- [ ] Cerrar pestañas y ventanas que no tengan que ver
- [ ] Probar que el micrófono se escuche
- [ ] Ensayar una vez con cronómetro: son 3 minutos y se pasan rápido
