// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponServerBase.h"

#include "NetshootCharacterBase.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AWeaponServerBase::AWeaponServerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
       
	ServerWeaponMesh=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ServerWeaponMesh"));
	SetRootComponent(ServerWeaponMesh);
	SphereCollision=CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(ServerWeaponMesh);
     
	// ServerWeaponMesh->SetSimulatePhysics(true);
	// ServerWeaponMesh->SetEnableGravity(true);
	ServerWeaponMesh->SetOwnerNoSee(true);

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this,&AWeaponServerBase::OnComponetBeginOverlap);

	SetReplicates(true);
}

// Called when the game starts or when spawned
void AWeaponServerBase::BeginPlay()
{
	Super::BeginPlay();
	
}



// Called every frame
void AWeaponServerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AWeaponServerBase::OnComponetBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ANetshootCharacterBase*  Player=Cast<ANetshootCharacterBase>(OtherActor);
		if (Player)
		{
			Player->EquipPrimary(this);
		}
	}
}

void AWeaponServerBase::MuliticastShootingEffect_Implementation()
{
	if (GetOwner()!=UGameplayStatics::GetPlayerCharacter(GetWorld(),0))
	{
		UGameplayStatics::SpawnEmitterAttached(FireServerPartical,ServerWeaponMesh,TEXT("Muzzle"));

		UGameplayStatics::PlaySoundAtLocation(GetWorld(),Sound3D,GetActorLocation(),GetActorRotation());
		
	}
}

bool AWeaponServerBase::MuliticastShootingEffect_Validate()
{
	return true;
}

void AWeaponServerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME_CONDITION(AWeaponServerBase,ClipCurrentAmmon,COND_None);
}
